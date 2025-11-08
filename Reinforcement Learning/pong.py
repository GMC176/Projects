import argparse # plots
import collections
import random
import time
import numpy as np
import gym
import gymnasium as gymn   
import torch
import torch.nn as nn
import torch.optim as optim
import cv2
from tqdm import trange
from collections import deque
import matplotlib.pyplot as plt
from tqdm import tqdm
if not hasattr(np, 'bool8'): # fixing bool issue for older python
    np.bool8 = np.bool_

env = gym.make("Pong-v4", render_mode = "rgb_array") # pong environment
print("Observation space:", env.observation_space)
print("Action space:", env.action_space)
print("Action meanings:", env.unwrapped.get_action_meanings())

transitions = []
for episode in range(20): # random transitions
    state, info = env.reset()
    done = False
    while not done:
        action = env.action_space.sample()
        next_state, reward, done, truncated, _ = env.step(action)
        transitions.append((state, action, reward, next_state, done))
        state = next_state

print(f"Collected {len(transitions)} transitions.")

rewards = np.array([t[2] for t in transitions])
reward_frames = np.array([t[0] for t in transitions])[rewards != 0]
print(f"Found {len(reward_frames)} frames with nonzero rewards")

def extract_positions(frame): # extracting position of paddle and ball
    gray = np.mean(frame, axis=2)
    paddle1_mask = (gray[:, :40] > 200)   # left paddle
    paddle2_mask = (gray[:, -40:] > 200)  # right paddle
    ball_mask = (gray > 230)

    paddle1_y = np.mean(np.where(paddle1_mask)[0]) if np.any(paddle1_mask) else np.nan
    paddle1_x = np.mean(np.where(paddle1_mask)[1]) if np.any(paddle1_mask) else np.nan
    paddle2_y = np.mean(np.where(paddle2_mask)[0]) if np.any(paddle2_mask) else np.nan
    paddle2_x = np.mean(np.where(paddle2_mask)[1]) if np.any(paddle2_mask) else np.nan
    ball_y = np.mean(np.where(ball_mask)[0]) if np.any(ball_mask) else np.nan
    ball_x = np.mean(np.where(ball_mask)[1]) if np.any(ball_mask) else np.nan
    return paddle1_x, paddle1_y, paddle2_x, paddle2_y, ball_x, ball_y

paddle1_x, paddle1_y, paddle2_x, paddle2_y, ball_x, ball_y = [], [], [], [], [], []
for frame in reward_frames:
    p1x, p1y, p2x, p2y, bx, by = extract_positions(frame)
    paddle1_x.append(p1x)
    paddle1_y.append(p1y)
    paddle2_x.append(p2x)
    paddle2_y.append(p2y)
    ball_x.append(bx)
    ball_y.append(by)

# --- Plot ---
plt.figure(figsize=(8,6))
plt.scatter(paddle1_x, paddle1_y, c='blue', label='Left Paddle', alpha=0.5)
plt.scatter(paddle2_x, paddle2_y, c='red', label='Right Paddle', alpha=0.5)
plt.scatter(ball_x, ball_y, c='green', label='Ball', alpha=0.5)
plt.xlabel('X Position (pixels)')
plt.ylabel('Y Position (pixels)')
plt.title('Paddle and Ball Positions at Reward Events (Pong)')
plt.legend()
plt.gca().invert_yaxis()  # top-left origin in Pong frames
plt.show()

# plotting reward distribution
plt.figure(figsize=(6,4))
plt.hist(rewards, bins=np.arange(-1.5, 2.5, 1), align='left', rwidth=0.8, color='purple')
plt.xticks([-1, 0, 1])
plt.xlabel('Reward')
plt.ylabel('Frequency')
plt.title('Distribution of Rewards in Pong Transitions')
plt.show()

# difference of positions between paddle and ball at reward frames
paddle1_ball_diff_y = np.array(paddle1_y) - np.array(ball_y)
paddle2_ball_diff_y = np.array(paddle2_y) - np.array(ball_y)
plt.figure(figsize=(8,6))
plt.scatter(paddle1_ball_diff_y, rewards[rewards != 0], c='blue', label='Left Paddle vs Ball', alpha=0.5)
plt.scatter(paddle2_ball_diff_y, rewards[rewards != 0], c='red', label='Right Paddle vs Ball', alpha=0.5)
plt.xlabel('Vertical Position Difference (Paddle - Ball)')
plt.ylabel('Reward')
plt.title('Paddle-Ball Vertical Position Difference vs Reward')
plt.axhline(0, color='black', linestyle='--', linewidth=0.8)
plt.legend()
plt.show()

import gymnasium as gym
import numpy as np
import random
import torch
import torch.nn as nn
import torch.optim as optim
import cv2
from collections import deque
import matplotlib.pyplot as plt

device = torch.device("cuda" if torch.cuda.is_available() else "cpu") # using gpu 
print("Using device:", device)

if not hasattr(np, 'bool8'):
    np.bool8 = np.bool_

env = gym.make("Pong-v4", render_mode="rgb_array")
print("Action meanings:", env.unwrapped.get_action_meanings())

class ReplayBuffer: # replay buffer class
    def __init__(self, capacity):
        self.buffer = deque(maxlen=capacity)
    def push(self, state, action, reward, next_state, done):
        self.buffer.append((state, action, reward, next_state, done))
    def sample(self, batch_size):
        batch = random.sample(self.buffer, batch_size)
        state, action, reward, next_state, done = zip(*batch)
        return (
            np.stack(state),
            np.array(action),
            np.array(reward),
            np.stack(next_state),
            np.array(done, dtype=np.float32)
        )
    def __len__(self):
        return len(self.buffer)

class QNetwork(nn.Module): # q network class
    def __init__(self, input_shape, num_actions):
        super(QNetwork, self).__init__()
        self.conv = nn.Sequential(
            nn.Conv2d(input_shape[0], 16, kernel_size=4, stride=2),
            nn.LeakyReLU(0.01),
            nn.Conv2d(16, 32, kernel_size=3, stride=2),
            nn.LeakyReLU(0.01),
            nn.Conv2d(32, 64, kernel_size=2, stride=2),
            nn.LeakyReLU(0.01)
        )
        conv_out_size = self._get_conv_out(input_shape)
        self.fc = nn.Sequential(
            nn.Linear(conv_out_size, 128),
            nn.LeakyReLU(0.01),
            nn.Linear(128, 128),
            nn.LeakyReLU(0.01),
            nn.Linear(128, num_actions)
        )
        self.apply(self._weights_init)

    def _get_conv_out(self, shape):
        x = torch.zeros(1, *shape)
        o = self.conv(x)
        return int(np.prod(o.size()))

    def _weights_init(self, m):
        if isinstance(m, nn.Conv2d):
            nn.init.kaiming_normal_(m.weight, nonlinearity='leaky_relu')
            if m.bias is not None:
                nn.init.constant_(m.bias, 0)
        elif isinstance(m, nn.Linear):
            nn.init.xavier_uniform_(m.weight)
            if m.bias is not None:
                nn.init.constant_(m.bias, 0)

    def forward(self, x):
        x = x.float() / 255.0
        conv_out = self.conv(x).view(x.size(0), -1)
        return self.fc(conv_out)

def preprocess_frame(frame): # preprocessing frame
    gray = cv2.cvtColor(frame, cv2.COLOR_RGB2GRAY)
    gray = np.fliplr(gray)  # flip horizontally so agent paddle is on correct side
    cropped = gray[34:194, :]
    resized = cv2.resize(cropped, (84, 84), interpolation=cv2.INTER_AREA)
    return resized.astype(np.float32)

def preprocess_with_diff(curr_frame, prev_frame):
    return curr_frame - prev_frame

def get_ball_and_paddle_pos(gray_frame):
    # Detect ball (bright small object)
    ball_mask = cv2.inRange(gray_frame, 200, 255)
    ball_coords = cv2.findNonZero(ball_mask)
    ball_pos = None
    if ball_coords is not None:
        avg = np.mean(ball_coords, axis=0)[0]
        ball_pos = (int(avg[0]), int(avg[1]))

    paddle_region = gray_frame[:, 78:84]
    paddle_mask = cv2.inRange(paddle_region, 200, 255)
    paddle_coords = cv2.findNonZero(paddle_mask)
    paddle_pos = None
    if paddle_coords is not None:
        avg = np.mean(paddle_coords, axis=0)[0]
        paddle_pos = (int(avg[0] + 78), int(avg[1]))

    return ball_pos, paddle_pos

state, info = env.reset() # initialising our enviroment
frame = preprocess_frame(state)
height, width = frame.shape
input_channels = 1

q_network = QNetwork((input_channels, height, width), env.action_space.n).to(device)
target_network = QNetwork((input_channels, height, width), env.action_space.n).to(device)
target_network.load_state_dict(q_network.state_dict())
target_network.eval()

optimizer = optim.Adam(q_network.parameters(), lr=1e-4)
criterion = nn.SmoothL1Loss()
replay_buffer = ReplayBuffer(10000)


batch_size = 32
gamma = 0.99
epsilon_start = 1.0
epsilon_final = 0.05
epsilon_decay = 0.996
target_update_freq = 5000
num_episodes = 2000

total_steps = 0
episode_rewards, mean_rewards, steps_list = [], [], []
running_window = deque(maxlen=20)
hit_visualizations = 0  # Counter to limit visualizations

for episode in range(num_episodes): # training loop
    state, info = env.reset()
    frame = preprocess_frame(state)
    prev_frame = frame
    done = False
    episode_reward = 0
    num_steps_in_episode = 0
    hit_count = 0

    while not done:
        num_steps_in_episode += 1
        total_steps += 1

        curr_frame = preprocess_frame(state)
        diff_frame = preprocess_with_diff(curr_frame, prev_frame)
        prev_frame = curr_frame
        input_state = torch.tensor(diff_frame, dtype=torch.float32).unsqueeze(0).unsqueeze(0).to(device)

        epsilon = max(epsilon_final, epsilon_start * (epsilon_decay ** episode))
        if random.random() < epsilon:
            action = env.action_space.sample()
        else:
            with torch.no_grad():
                q_values = q_network(input_state)
                action = q_values.argmax(dim=1).item()

        next_state, reward, done, truncated, _ = env.step(action)
        done = done or truncated

        shaped_reward = np.clip(reward, -1.0, 1.0)

        ball_pos, paddle_pos = get_ball_and_paddle_pos(curr_frame) # checking for ball hit
        if ball_pos and paddle_pos:
            ball_x, ball_y = ball_pos
            paddle_x, paddle_y = paddle_pos
            
            if ball_x > 70 and abs(ball_y - paddle_y) < 7 and ball_x <= paddle_x: # hit condition
                shaped_reward += 0.1  # Small reward for being near ball
                hit_count += 1
                
                if hit_visualizations < 3:
                    hit_visualizations += 1
                    # Create visualization
                    vis = cv2.cvtColor(curr_frame.astype(np.uint8), cv2.COLOR_GRAY2BGR)
                    cv2.circle(vis, ball_pos, 3, (0, 0, 255), -1)  # Red ball
                    cv2.circle(vis, paddle_pos, 3, (0, 255, 0), -1)  # Green paddle
                    
                    
                    cv2.putText(vis, f"Ball: {ball_pos}", (5, 15), 
                               cv2.FONT_HERSHEY_SIMPLEX, 0.4, (255, 255, 255), 1)
                    cv2.putText(vis, f"Paddle: {paddle_pos}", (5, 30), 
                               cv2.FONT_HERSHEY_SIMPLEX, 0.4, (255, 255, 255), 1)
                    cv2.putText(vis, f"Distance: {abs(ball_y - paddle_y)}", (5, 45), 
                               cv2.FONT_HERSHEY_SIMPLEX, 0.4, (255, 255, 255), 1)
                    
                    plt.figure(figsize=(6, 6))
                    plt.title(f"Hit Detection #{hit_visualizations}\nEpisode {episode+1}, Step {num_steps_in_episode}")
                    plt.imshow(cv2.cvtColor(vis, cv2.COLOR_BGR2RGB))
                    plt.axis("off")
                    plt.tight_layout()
                    plt.show()
                    print(f"Visualized hit #{hit_visualizations}: Ball at {ball_pos}, Paddle at {paddle_pos}")

        next_frame = preprocess_frame(next_state)
        next_frame_diff = preprocess_with_diff(next_frame, curr_frame)
        replay_buffer.push(diff_frame, action, shaped_reward, next_frame_diff, done)
        state = next_state
        episode_reward += reward

        # --- Training ---
        if len(replay_buffer) >= batch_size:
            states_b, actions_b, rewards_b, next_states_b, dones_b = replay_buffer.sample(batch_size)
            states_b = torch.tensor(states_b, dtype=torch.float32).unsqueeze(1).to(device)
            actions_b = torch.tensor(actions_b, dtype=torch.int64).unsqueeze(1).to(device)
            rewards_b = torch.tensor(rewards_b, dtype=torch.float32).unsqueeze(1).to(device)
            next_states_b = torch.tensor(next_states_b, dtype=torch.float32).unsqueeze(1).to(device)
            dones_b = torch.tensor(dones_b, dtype=torch.float32).unsqueeze(1).to(device)

            q_values = q_network(states_b).gather(1, actions_b)

            with torch.no_grad():
                next_actions = q_network(next_states_b).argmax(1).unsqueeze(1)
                next_q_values = target_network(next_states_b).gather(1, next_actions)
                target_q_values = rewards_b + gamma * next_q_values * (1 - dones_b)

            loss = criterion(q_values, target_q_values)
            optimizer.zero_grad()
            loss.backward()
            torch.nn.utils.clip_grad_norm_(q_network.parameters(), 1.0)
            optimizer.step()

            if total_steps % target_update_freq == 0:
                target_network.load_state_dict(q_network.state_dict())
                print(f"Target network updated at step {total_steps}")

    episode_rewards.append(episode_reward)
    running_window.append(episode_reward)
    mean_reward = np.mean(running_window)
    mean_rewards.append(mean_reward)
    steps_list.append(total_steps)

    print(f"Ep {episode+1} | Steps: {num_steps_in_episode} | Reward: {episode_reward:.1f} | Mean(20): {mean_reward:.2f} | Eps: {epsilon:.3f} | Hits: {hit_count}")

plt.figure(figsize=(8,5))
plt.plot(steps_list, mean_rewards, label='Mean (20 eps)')
plt.xlabel("Steps")
plt.ylabel("Mean Reward (20 eps)")
plt.title("Double DQN Training on Pong-v4")
plt.legend()
plt.grid(alpha=0.5)
plt.show()

print("\n=== TRAINING COMPLETE ===")
print(f"Episodes: {len(episode_rewards)} | Total steps: {total_steps}")
print(f"Final mean reward: {mean_rewards[-1]:.2f}")
print(f"Total hits visualized: {hit_visualizations}")
