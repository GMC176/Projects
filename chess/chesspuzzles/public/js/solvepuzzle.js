document.addEventListener('DOMContentLoaded', () => {
    const puzzleList = document.getElementById('puzzleList');
    
    puzzleList.querySelectorAll('.puzzle').forEach(puzzleDiv => {
        const solutionForm = puzzleDiv.querySelector('.solutionForm');
        const solutionInput = puzzleDiv.querySelector('input[type="text"]');
        const messageDiv = puzzleDiv.querySelector('.message');
        solutionForm.addEventListener('submit', (event) => {
            event.preventDefault();
            const userSolution = solutionInput.sub;
            const correctSolution = puzzleDiv.getAttribute('data-solution');
            console.log(correctSolution);
            if (userSolution === correctSolution) {
                messageDiv.textContent = 'Correct!';
            } else {
                messageDiv.textContent = 'Incorrect, try again.';
            }
        });
    });
});

