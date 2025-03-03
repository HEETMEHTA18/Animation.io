// Get the button element
const modeToggleButton = document.getElementById('mode-toggle');

// Function to toggle the modes
function toggleMode() {
  document.body.classList.toggle('dark-mode');
  // Store the mode preference in localStorage
  const currentMode = document.body.classList.contains('dark-mode') ? 'dark' : 'light';
  localStorage.setItem('theme', currentMode);
}

// Set initial mode based on localStorage (if available)
const savedMode = localStorage.getItem('theme');
if (savedMode === 'dark') {
  document.body.classList.add('dark-mode');
}

// Add event listener for the button
modeToggleButton.addEventListener('click', toggleMode);

// No JavaScript is needed for functionality, but if you want to trigger something upon toggling, use this

const checkbox = document.getElementById('checkbox');

checkbox.addEventListener('change', (event) => {
  if (event.target.checked) {
    console.log('Dark mode or custom mode activated!');
  } else {
    console.log('Light mode or default mode activated!');
  }
});
