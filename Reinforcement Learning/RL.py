# %%
import numpy as np
import matplotlib.pyplot as plt
import gymnasium as gym
import torch
import torch.nn as nn
import torch.optim as optim
from torch.distributions import Categorical
import pandas as pd

# %%
import argparse

parser = argparse.ArgumentParser(description="Policy Gradient with optional Reward-to-Go and Advantage Normalization") # argument parser for command line options

parser.add_argument("--env_name", type=str, default="CartPole-v1", help="Gym environment name")
parser.add_argument("--num_iterations", type=int, default=500, help="Number of iterations to train")
parser.add_argument("--reward_to_go", action="store_true", help="Use reward-to-go instead of total return")
parser.add_argument("--baseline", action="store_true", help="Use baseline network for advantage")
parser.add_argument("--normalize_advantages", action="store_true", help="Normalize advantages")
parser.add_argument("--discount_factor", type=float, default=0.99, help="Discount factor for returns")
parser.add_argument("--learning_rate", type=float, default=0.001, help="Learning rate for policy and baseline networks")
parser.add_argument("--batch_size", type=int, default=5, help="Number of episodes per policy update")

args = parser.parse_args()
# %%
'''env = gym.make("CartPole-v1", render_mode="rgb_array")
num_actions = env.action_space.n
num_states = env.observation_space.shape[0]
print(f"Number of actions: {num_actions}, Number of states: {num_states}")

print("action space:", env.action_space)
print("observation space:", env.observation_space)

print("meaning of actions: 0 - push cart to the left, 1 - push cart to the right")'''

# %%
def cartpole(): # CARTPOLE ENVIRONMENT FUNCTION

    env = gym.make("CartPole-v1", render_mode="rgb_array") # printing information about environment, part (a)
    num_actions = env.action_space.n
    num_states = env.observation_space.shape[0]
    print(f"Number of actions: {num_actions}, Number of states: {num_states}") # printing information about environment, part (a)

    print("action space:", env.action_space) # printing information about environment
    print("observation space:", env.observation_space) # printing information about environment

    print("meaning of actions: 0 - push cart to the left, 1 - push cart to the right")

    transitions = []

    for episode in range(20): # collecting data with random policy
        state, _ = env.reset() # resetting environment for new episode
        done = False
        reward_episode = 0
        time_steps = 0
        max_right = -np.inf
        max_left = np.inf
        while not done: # running episode until done
            time_steps += 1
            action = env.action_space.sample() # getting sample/random action from action space
            next_state, reward, terminated, truncated, info = env.step(action) # taking action and getting next state, reward, done flag and info
            max_right = max(max_right, next_state[0]) # updating max positions
            max_left = min(max_left, next_state[0])
            done = terminated or truncated
            state = np.array(state, dtype=np.float32).flatten()
            next_state = np.array(next_state, dtype=np.float32).flatten()
            transitions.append((state, action, reward, next_state, done)) # storing transition
            state = next_state
            reward_episode += reward


        print(f"Episode {episode + 1} finished, total reward: {reward_episode =}, time steps: {time_steps} , max right: {max_right:.2f}, max left: {max_left:.2f}")

    data_list = [] # storing data in list of dictionaries

    for transition in transitions: # iterating through transitions and saving in list
        state, action, reward, next_state, done = transition
        data_list.append(
            {
                "state": state,
                "action": action,
                "reward": reward,
                "next_state": next_state,
                "done": done,
            }
        )



# %%


    columns = ["cart_pos", "cart_vel", "pole_angle", "pole_ang_vel", "action", "reward", "done"] # defining columns for dataframe
    data = []

    for (s, a, r, ns, d) in transitions: # appending data to list
        data.append([s[0], s[1], s[2], s[3], a, r, d])

    df = pd.DataFrame(data, columns=columns)

    print("\nAction counts:")
    print(df["action"].value_counts()) # counting how many times particular action was taken


    plt.figure(figsize=(10,6)) # cartpole state variable visualization
    plt.subplot(3,1,1)
    plt.plot(df["cart_pos"], label="Cart Position")
    plt.ylabel("Position")
    plt.legend()

    plt.subplot(3,1,2) # pole angle plot
    plt.plot(df["pole_angle"], label="Pole Angle", color="orange")
    plt.ylabel("Angle (radians)")
    plt.legend()

    plt.figure(figsize=(8,5)) # reward position scatter plot
    plt.scatter(df["cart_pos"], df["reward"], alpha=0.5)
    plt.xlabel("Cart Position")
    plt.ylabel("Reward")
    plt.title("Reward vs Cart Position (Random Policy)")
    plt.show()

    df_not_1 = df[df["done"] != False]
    print(df_not_1) # filtering dataframe for non-1 rewards
    plt.figure(figsize=(8,5))
    plt.scatter(df_not_1["cart_vel"], df_not_1["reward"], alpha=0.5, color="red")
    plt.xlabel("Cart Position")
    plt.ylabel("Reward")
    plt.title("Reward vs Cart Position (Non-1 Rewards)")
    plt.show()

# comment: In CartPole-v1, the reward is always 1 for each time step the pole remains upright, so there is no variation in reward to correlate with state variables.
# observations:
# For every time step episode is running, we get a reward of 1
# In our random simulation, we observed different numbers as observed reward per episode
# Mostly our episodes ended because of the pole angle exceeding the threshold rather than cart position
# we never observed the cart position exceeding the threshold in our random policy runs
# This suggests that in a random policy, the pole angle is a more critical factor leading to episode termination than cart position.


# %%




# %%
    class PolicyNetwork(nn.Module): # defining policy network
        def __init__(self, state_size, action_size, hidden_size=128):
            super(PolicyNetwork, self).__init__()
            self.net = nn.Sequential(
                nn.Linear(state_size, hidden_size),
                nn.ReLU(),
                nn.Linear(hidden_size, hidden_size),
                nn.ReLU(),
                nn.Linear(hidden_size, action_size),
                nn.Softmax(dim=-1)
            )

        def forward(self, x):
            return self.net(x)
        
    class BaselineNetwork(nn.Module): # defining baseline network
        def __init__(self, state_size, hidden_size=128):
            super(BaselineNetwork, self).__init__()
            self.net = nn.Sequential(
                nn.Linear(state_size, hidden_size),
                nn.ReLU(),
                nn.Linear(hidden_size, 1)
            )

        def forward(self, x):
            return self.net(x).squeeze(-1)
        
    # initializing policy and baseline networks, optimizers, and other variables
    policy_net = PolicyNetwork(num_states, num_actions)
    baseline_net = BaselineNetwork(num_states)
    learning_rate = args.learning_rate
    policy_optimizer = optim.Adam(policy_net.parameters(), lr=learning_rate)
    baseline_optimizer = optim.Adam(baseline_net.parameters(), lr=learning_rate)
    reward_to_go_condition = args.reward_to_go
    baseline_condition = args.baseline
    normalize_advantages = args.normalize_advantages
    episodes = args.num_iterations * args.batch_size
    discount_factor = args.discount_factor 
    batch_size = args.batch_size
    all_trajectories = []
    average_rewards = []
    episode_rewards = []

    def total_reward(trajectory, discount_factor): # computing total reward for trajectory
        reward = 0
        discount = 1
        l = len(trajectory)
        for state, action,r in trajectory:
            reward += r * discount
            discount *= discount_factor
        return np.array([reward] * l, dtype=np.float32)

    def reward_to_go(trajectory, time_steps, discount_factor): # computing reward-to-go for trajectory
        num_steps = len(trajectory)
        future_reward = 0
        discount = 1
        rewardtogo = np.zeros(num_steps)
        for i in reversed(range(num_steps)):
            state, action, r = trajectory[i]
            future_reward = r + discount_factor * future_reward
            rewardtogo[i] = future_reward
        return rewardtogo

    def compute_baseline(trajectory, Gt, baseline_net): # computing baseline values for trajectory
        baselines = []
        for state, action, reward in trajectory:
            state_tensor = torch.FloatTensor(state).unsqueeze(0)
            baseline_value = baseline_net(state_tensor).item()
            baselines.append(baseline_value)
        return np.array(baselines)



    for episode in range(episodes): # training loop, collecting trajectories and updating policy and baseline networks
        trajectory = [] # reseting variables particular to each episode
        state, _ = env.reset()
        done = False
        time_steps = 0
        episode_reward = 0
        while not done: # running episode until done, keep collecting trajectory untill terminated or truncated
            state_tensor = torch.FloatTensor(state).unsqueeze(0) 
            action_probs = policy_net(state_tensor) 
            dist = Categorical(action_probs) 
            action = dist.sample().item() 
            next_state, reward, terminated, truncated, info = env.step(action)
            episode_reward += reward
            done = terminated or truncated
            theta = next_state[2]
            train_reward = (1.0 - 2*abs(theta)/0.209) # shaping reward based on pole angle
            if terminated or truncated: # applying terminal penalty for early convergence
                train_reward -= 5.0
            trajectory.append((state, action, train_reward))
            state = next_state
            time_steps += 1

        all_trajectories.append(trajectory) # appending trajectory and episode reward
        episode_rewards.append(episode_reward)

        if (episode + 1) % batch_size == 0: # updating policy and baseline networks after batch of episodes
            states, actions, rewards = [], [], []
            batch_states = []
            batch_actions = []
            batch_advantages = []
            for traj in all_trajectories: # computing advantages for each trajectory
                if reward_to_go_condition:
                    Gt = reward_to_go(traj, len(traj), discount_factor)
                else:
                    Gt = total_reward(traj, discount_factor)
            
                baseline_values = np.zeros(len(traj))
                if baseline_condition: # checking baseline condition
                    baseline_values = compute_baseline(traj, Gt, baseline_net)

                advantages = Gt - baseline_values

                if normalize_advantages: # checking advantage normalization condition
                    advantages = (advantages - np.mean(advantages)) / (np.std(advantages) + 1e-8)

                for (s, a, _), adv in zip(traj, advantages): # appending states, actions and advantages to batch lists
                    batch_states.append(s)
                    batch_actions.append(a)
                    batch_advantages.append(adv)


            states_t = torch.FloatTensor(batch_states)
            actions_t = torch.LongTensor(batch_actions)
            advantages_t = torch.FloatTensor(batch_advantages)

            policy_net.train() # updating policy network
            policy_optimizer.zero_grad()
            dist = Categorical(policy_net(states_t))
            log_probs = dist.log_prob(actions_t)
            loss = -(log_probs * advantages_t).mean()
            loss.backward()
            policy_optimizer.step()

            if baseline_condition: # updating baseline network if true
                baseline_net.train()
                baseline_optimizer.zero_grad()
                baseline_values_t = baseline_net(states_t)
                Gt_all = torch.cat([torch.FloatTensor(Gt) for traj in all_trajectories for Gt in [reward_to_go(traj, len(traj), discount_factor) if reward_to_go_condition else total_reward(traj, discount_factor)]])
                baseline_loss = nn.MSELoss()(baseline_values_t, Gt_all)
                baseline_loss.backward()
                baseline_optimizer.step()

            all_trajectories = []


        print(f"Episode {episode + 1} completed. Total Reward: {episode_reward}, Time Steps: {time_steps}")

    for i in range(0, len(episode_rewards), args.batch_size): # computing average rewards per batch
        avg_reward = np.mean(episode_rewards[i:i+args.batch_size])
        average_rewards.append(avg_reward)

    '''filename = f"results_norm_{normalize_advantages}_r2g_{reward_to_go_condition}_batchsize_{batch_size}.npy"
    np.save(filename, np.array(average_rewards))
    print(f"Saved results to {filename}")'''

def lunarlander(): # LUNARLANDER ENVIRONMENT FUNCTION

    env = gym.make("LunarLander-v3", render_mode="rgb_array") # printing information about environment, part (a)
    num_actions = env.action_space.n
    num_states = env.observation_space.shape[0]
    print(f"Number of actions: {num_actions}, Number of states: {num_states}")

    print("action space:", env.action_space)
    print("observation space:", env.observation_space)

    print("meaning of actions: 0 - do nothing, 1 - fire left engine, 2 - fire main engine, 3 - fire right engine")

    transitions = []

    for episode in range(20): # collecting data with random policy
        state, _ = env.reset()
        done = False
        reward_episode = 0
        time_steps = 0
        max_right = -np.inf # tracking max positions in all directions
        max_left = np.inf
        max_up = -np.inf
        max_down = np.inf
        while not done: # running episode until done
            time_steps += 1
            action = env.action_space.sample()
            next_state, reward, terminated, truncated, info = env.step(action)
            max_right = max(max_right, next_state[0])
            max_left = min(max_left, next_state[0])
            max_up = max(max_up, next_state[1])
            max_down = min(max_down, next_state[1])
            done = terminated or truncated
            state = np.array(state, dtype=np.float32).flatten()
            next_state = np.array(next_state, dtype=np.float32).flatten()
            transitions.append((state, action, reward, next_state, done))
            state = next_state
            reward_episode += reward

        print(f"Episode {episode + 1} finished, total reward: {reward_episode =}, time steps: {time_steps} , max right: {max_right:.2f}, max left: {max_left:.2f}, max up: {max_up:.2f}, max down: {max_down:.2f}")
        #print(f"Episode {episode + 1} finished, total reward: {reward_episode =}, time steps: {time_steps} , max right: {max_right:.2f}, max left: {max_left:.2f}")

    data_list = []

    for transition in transitions: # iterating through transitions and saving in list
        state, action, reward, next_state, done = transition
        data_list.append(
            {
                "state": state,
                "action": action,
                "reward": reward,
                "next_state": next_state,
                "done": done,
            }
        )



# %%


    columns = ["pos_x", "pos_y", "vel_x", "vel_y", "angle", "angular_vel", "leg1_contact", "leg2_contact", "action", "reward", "done"] # defining columns for dataframe
    data = []

    for (s, a, r, ns, d) in transitions: # appending data to list
        data.append([s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7], a, r, d])

    df = pd.DataFrame(data, columns=columns)



    print("Action counts:") # counting how many times particular action was taken
    print(df["action"].value_counts())

    plt.figure(figsize=(10,6)) # x position, y position and angle visualization
    plt.subplot(3,1,1)
    plt.plot(df["pos_x"], label="Lander X Position")
    plt.ylabel("Position X")
    plt.legend()
    plt.subplot(3,1,2)
    plt.plot(df["pos_y"], label="Lander Y Position", color="orange")
    plt.ylabel("Position Y")
    plt.legend()
    plt.subplot(3,1,3)
    plt.plot(df["angle"], label="Lander Angle", color="green")
    plt.ylabel("Angle (radians)")
    plt.legend()
    plt.show()

    plt.figure(figsize=(8,5)) # reward vs X position scatter plot
    plt.scatter(df["pos_x"], df["reward"], alpha=0.5)
    plt.xlabel("Lander X Position")
    plt.ylabel("Reward")
    plt.title("Reward vs Lander X Position (Random Policy)")
    plt.show()

    plt.figure(figsize=(8,5)) # reward vs Y position scatter plot
    plt.scatter(df["pos_y"], df["reward"], alpha=0.5, color="red")
    plt.xlabel("Lander Y Position")
    plt.ylabel("Reward")
    plt.title("Reward vs Lander Y Position (Random Policy)")
    plt.show()

    plt.figure(figsize=(8,5)) # reward vs angle scatter plot
    plt.scatter(df["angle"], df["reward"], alpha=0.5, color="green")
    plt.xlabel("Lander Angle")
    plt.ylabel("Reward")
    plt.title("Reward vs Lander Angle (Random Policy)")
    plt.show()

    plt.figure(figsize=(8,5)) # reward vs X velocity scatter plot
    plt.scatter(df["vel_x"], df["reward"], alpha=0.5, color="purple")
    plt.xlabel("Lander X Velocity")
    plt.ylabel("Reward")
    plt.title("Reward vs Lander X Velocity (Random Policy)")
    plt.show()  

    plt.figure(figsize=(8,5)) # reward vs Y velocity scatter plot
    plt.scatter(df["vel_y"], df["reward"], alpha=0.5, color="brown")
    plt.xlabel("Lander Y Velocity")
    plt.ylabel("Reward")
    plt.title("Reward vs Lander Y Velocity (Random Policy)")
    plt.show()

    plt.figure(figsize=(8,5)) # reward vs angular velocity scatter plot
    plt.scatter(df["angular_vel"], df["reward"], alpha=0.5, color="orange")
    plt.xlabel("Lander Angular Velocity")
    plt.ylabel("Reward")
    plt.title("Reward vs Lander Angular Velocity (Random Policy)")
    plt.show()

    df_not_1 = df[df["done"] != False] # filtering dataframe for non-1 rewards
    plt.figure(figsize=(8,5)) # reward vs X position scatter plot for non-1 rewards
    plt.scatter(df_not_1["pos_x"], df_not_1["reward"], alpha=0.5, color="red")
    plt.xlabel("Lander X Position")
    plt.ylabel("Reward")
    plt.title("Reward vs Lander X Position (Non-1 Rewards)")
    plt.show()

    plt.figure(figsize=(8,5)) # reward vs Y position scatter plot for non-1 rewards
    plt.scatter(df_not_1["pos_y"], df_not_1["reward"], alpha=0.5, color="green")
    plt.xlabel("Lander Y Position")
    plt.ylabel("Reward")
    plt.title("Reward vs Lander Y Position (Non-1 Rewards)")
    plt.show() 
    plt.figure(figsize=(8,5)) # reward vs angle scatter plot for non-1 rewards
    plt.scatter(df_not_1["angle"], df_not_1["reward"], alpha=0.5, color="blue")
    plt.xlabel("Lander Angle")
    plt.ylabel("Reward")
    plt.title("Reward vs Lander Angle (Non-1 Rewards)")
    plt.show() 





# comment: In CartPole-v1, the reward is always 1 for each time step the pole remains upright, so there is no variation in reward to correlate with state variables.
# observations:
# For every time step episode is running, we get a reward of 1
# In our random simulation, we observed different numbers as observed reward per episode
# Mostly our episodes ended because of the pole angle exceeding the threshold rather than cart position
# we never observed the cart position exceeding the threshold in our random policy runs
# This suggests that in a random policy, the pole angle is a more critical factor leading to episode termination than cart position.


# %%




# %%
    class PolicyNetwork(nn.Module): # defining policy network
        def __init__(self, state_size, action_size, hidden_size=128):
            super(PolicyNetwork, self).__init__()
            self.net = nn.Sequential(
                nn.Linear(state_size, hidden_size),
                nn.ReLU(),
                nn.Linear(hidden_size, hidden_size),
                nn.ReLU(),
                nn.Linear(hidden_size, action_size),
                nn.Softmax(dim=-1)
            )

        def forward(self, x):
            return self.net(x)
        
    class BaselineNetwork(nn.Module): # defining baseline network
        def __init__(self, state_size, hidden_size=128):
            super(BaselineNetwork, self).__init__()
            self.net = nn.Sequential(
                nn.Linear(state_size, hidden_size),
                nn.ReLU(),
                nn.Linear(hidden_size, 1)
            )

        def forward(self, x):
            return self.net(x).squeeze(-1)
        
    policy_net = PolicyNetwork(num_states, num_actions) # initializing policy and baseline networks, optimizers, and other variables
    baseline_net = BaselineNetwork(num_states)
    learning_rate = args.learning_rate
    policy_optimizer = optim.Adam(policy_net.parameters(), lr=learning_rate)
    baseline_optimizer = optim.Adam(baseline_net.parameters(), lr=learning_rate)
    reward_to_go_condition = args.reward_to_go
    baseline_condition = args.baseline
    normalize_advantages = args.normalize_advantages
    episodes = args.num_iterations * args.batch_size
    discount_factor = args.discount_factor 
    batch_size = args.batch_size
    all_trajectories = []
    average_rewards = []
    episode_rewards = []

    def total_reward(trajectory, discount_factor): # computing total reward for trajectory
        reward = 0
        discount = 1
        l = len(trajectory)
        for state, action,r in trajectory:
            reward += r * discount
            discount *= discount_factor
        return np.array([reward] * l, dtype=np.float32)

    def reward_to_go(trajectory, time_steps, discount_factor): # computing reward-to-go for trajectory
        num_steps = len(trajectory)
        future_reward = 0
        discount = 1
        rewardtogo = np.zeros(num_steps)
        for i in reversed(range(num_steps)):
            state, action, r = trajectory[i]
            future_reward = r + discount_factor * future_reward
            rewardtogo[i] = future_reward
        return rewardtogo

    def compute_baseline(trajectory, Gt, baseline_net): # computing baseline values for trajectory
        baselines = []
        for state, action, reward in trajectory:
            state_tensor = torch.FloatTensor(state).unsqueeze(0)
            baseline_value = baseline_net(state_tensor).item()
            baselines.append(baseline_value)
        return np.array(baselines)



    for episode in range(episodes): # training loop, collecting trajectories and updating policy and baseline networks
        trajectory = []
        state, _ = env.reset()
        done = False
        time_steps = 0
        episode_reward = 0
        while not done: # running episode until done, keep collecting trajectory untill terminated or truncated
            state_tensor = torch.FloatTensor(state).unsqueeze(0) 
            action_probs = policy_net(state_tensor) 
            dist = Categorical(action_probs) 
            action = dist.sample().item() 
            next_state, reward, terminated, truncated, info = env.step(action)
            episode_reward += reward
            done = terminated or truncated
          #  theta = next_state[2]
            #train_reward = reward
            pos_x, pos_y, vel_x, vel_y, angle, angular_vel, left_contact, right_contact = next_state
            pad_x = 0.0   # typically landing pad at x=0; check env.metadata if different
            dist = np.sqrt((pos_x - pad_x)**2 + pos_y**2)
            phi = -dist - 0.1 * (abs(vel_x) + abs(vel_y)) - 0.5 * abs(angle)
            phi_next = - (np.sqrt((pos_x - pad_x)**2 + pos_y**2)) - 0.1 * (abs(vel_x) + abs(vel_y)) - 0.5 * abs(angle)

            shaping = discount_factor * phi_next - phi
            shaping_scale = 2.0
            shaping *= shaping_scale

            touchdown_bonus = 0.0 # bonus for leg contact
            if left_contact or right_contact: # one leg has contact
                touchdown_bonus = 100.0

            crash_penalty = -100.0 if (terminated and not (left_contact or right_contact)) else 0.0 # penalty for crash without leg contact

            train_reward = reward + 0.01 * shaping + 0.001 * touchdown_bonus + crash_penalty # shaping reward based on distance, velocity, angle, leg contact and crash
          #  if terminated or truncated:
          #      train_reward -= 5.0
            trajectory.append((state, action, train_reward))
            state = next_state
            time_steps += 1

        all_trajectories.append(trajectory)
        episode_rewards.append(episode_reward)

        if (episode + 1) % batch_size == 0: # updating policy and baseline networks after batch of episodes
            states, actions, rewards = [], [], []
            batch_states = []
            batch_actions = []
            batch_advantages = []
            for traj in all_trajectories:
                if reward_to_go_condition:
                    Gt = reward_to_go(traj, len(traj), discount_factor)
                else:
                    Gt = total_reward(traj, discount_factor)
            
                baseline_values = np.zeros(len(traj))
                if baseline_condition:
                    baseline_values = compute_baseline(traj, Gt, baseline_net)

                advantages = Gt - baseline_values

                if normalize_advantages:
                    advantages = (advantages - np.mean(advantages)) / (np.std(advantages) + 1e-8)

                for (s, a, _), adv in zip(traj, advantages):
                    batch_states.append(s)
                    batch_actions.append(a)
                    batch_advantages.append(adv)


            states_t = torch.FloatTensor(batch_states)
            actions_t = torch.LongTensor(batch_actions)
            advantages_t = torch.FloatTensor(batch_advantages)

            policy_net.train()
            policy_optimizer.zero_grad()
            dist = Categorical(policy_net(states_t))
            log_probs = dist.log_prob(actions_t)
            loss = -(log_probs * advantages_t).mean()
            loss.backward()
            policy_optimizer.step()

            if baseline_condition: # updating baseline network if true
                baseline_net.train()
                baseline_optimizer.zero_grad()
                baseline_values_t = baseline_net(states_t)
                Gt_all = torch.cat([torch.FloatTensor(Gt) for traj in all_trajectories for Gt in [reward_to_go(traj, len(traj), discount_factor) if reward_to_go_condition else total_reward(traj, discount_factor)]])
                baseline_loss = nn.MSELoss()(baseline_values_t, Gt_all)
                baseline_loss.backward()
                baseline_optimizer.step()

            all_trajectories = []


        print(f"Episode {episode + 1} completed. Total Reward: {episode_reward}, Time Steps: {time_steps}")

    for i in range(0, len(episode_rewards), args.batch_size): # computing average rewards per batch
        avg_reward = np.mean(episode_rewards[i:i+args.batch_size])
        average_rewards.append(avg_reward)

    '''filename = f"lunarlander_results_norm_{normalize_advantages}_r2g_{reward_to_go_condition}_batchsize_{batch_size}.npy"
    np.save(filename, np.array(average_rewards))
    print(f"Saved results to {filename}")'''
    
if __name__ == "__main__":
    if args.env_name == "CartPole-v1": # CARTPOLE ENVIRONMENT
        cartpole()
    elif args.env_name == "LunarLander-v3": # LUNARLANDER ENVIRONMENT
        lunarlander()
    else:
        print("Unsupported environment. Please choose either 'CartPole-v1' or 'LunarLander-v2'.")




    

        

        



        