console.log("Hello from JS");
async function updateDuration() {
  try {
    const response = await fetch('/duration');
    if (!response.ok) {
        throw new Error('Server unreachable');
    }
    const json = await response.json();
    document.getElementById('duration').innerText = 'Duration: ' + json.duration
  } catch (error) {
      console.error('There was a problem with the fetch operation:', error);
  }
}

// Update the text box every second
// setInterval(updateDuration, 1000); // 1000 milliseconds = 1 second
