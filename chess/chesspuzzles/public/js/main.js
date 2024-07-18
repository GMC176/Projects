document.addEventListener('DOMContentLoaded', () => {
    const signupForm = document.getElementById('signupForm');
    const loginForm = document.getElementById('loginForm');

    signupForm.addEventListener('submit', (e) => {
        e.preventDefault();
        const loginId = document.getElementById('signupLoginId').value;
        const password = document.getElementById('signupPassword').value;

        fetch('/signup', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ loginId, password }),
        })
        .then(response => response.json())
        .then(data => {
            alert(data.message);
        })
        .catch(error => console.error('Error:', error));
    });

    loginForm.addEventListener('submit', (event) => {
        event.preventDefault(); // Prevent the default form submission

        const loginId = document.getElementById('loginLoginId').value;
        const password = document.getElementById('loginPassword').value;

        fetch('/login', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ loginId, password })
        })
        .then(response => response.json())
        .then(data => {
            if (data.success) {
                // Redirect to the welcome page with the username in the query string
                window.location.href = `/login.html?username=${encodeURIComponent(data.username)}`;
            } else {
                alert(data.message);
            }
        })
        .catch(error => console.error('Error:', error));
    });
});

