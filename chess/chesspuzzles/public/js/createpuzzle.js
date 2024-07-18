document.addEventListener('DOMContentLoaded', () => {
    const puzzleForm = document.getElementById('puzzleForm');

    puzzleForm.addEventListener('submit', (event) => {
    
        event.preventDefault(); // Prevent the default form submission
	console.log("abbbbbbbbbbbbbbbbbbbbbbbbbbbbbbsd");
        const fen = board2.fen();
        const solution = document.getElementById('solution').value;
        const num= document.getElementById('nummoves').value;
        const playby= document.getElementById('playSwitch').checked ? 1 : 0;

        fetch('/createpuzzle', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ fen, solution,num,playby })
        })
        .then(response => response.json())
        .then(data => {
            if (data.success) {
                // Puzzle creation was successful
                alert(data.message);
                // Optionally, redirect to another page or clear the form
                window.location.href = '/views/puzzle.html'; // Redirect to the puzzles page
            } else {
                // There was an error creating the puzzle
                alert(data.message);
            }
        })
        .catch(error => console.error('Error:', error));
    });
});

