// Define API key and chatbot ID
const apiKey = '43753f0b-431a-4b1c-xxxxxxx-xxxxx';
const chatbotId = '_8PrL19i1s2zxxxxxxxx';

// Function to send a message to the chatbot
async function sendMessage() {
    const messageInput = document.getElementById('message');
    const message = messageInput.value;
    const chatWindow = document.getElementById('chat-window');

    // Check if the input message is empty
    if (message.trim() === '') return;

    // Create a div element for user message and append it to the chat window
    const userMessageDiv = document.createElement('div');
    userMessageDiv.classList.add('chat-message', 'user-message');
    userMessageDiv.innerText = message;
    chatWindow.appendChild(userMessageDiv);
    chatWindow.scrollTop = chatWindow.scrollHeight;

    // Clear the input field after sending the message
    messageInput.value = '';

    // Create a placeholder for the bot message
    const botMessageDiv = document.createElement('div');
    botMessageDiv.classList.add('chat-message', 'bot-message');
    chatWindow.appendChild(botMessageDiv);
    chatWindow.scrollTop = chatWindow.scrollHeight;

    try {
        // Send the message to the chatbot API
        const response = await fetch('https://www.chatbase.co/api/v1/chat', {
            method: 'POST',
            headers: {
                'Authorization': `Bearer ${apiKey}`,
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({
                messages: [
                    { content: message, role: 'user' },
                ],
                chatbotId: chatbotId,
                stream: true,
                temperature: 0.5,
                model: 'gpt-4o',
            }),
        });

        // Check if the response is OK
        if (!response.ok) {
            const errorData = await response.json();
            throw new Error(errorData.message);
        }

        // Read the response stream and decode the text
        const reader = response.body.getReader();
        const decoder = new TextDecoder();
        let botMessage = '';

        // Function to read the stream data
        function read() {
            reader.read().then(({ done, value }) => {
                if (done) {
                    return;
                }

                // Append the decoded text to the bot message and update the chat window
                botMessage += decoder.decode(value);
                botMessageDiv.innerText = botMessage;
                chatWindow.scrollTop = chatWindow.scrollHeight;
                read();
            });
        }

        // Start reading the stream data
        read();
    } catch (error) {
        console.error('Error sending message to chatbot:', error);
    }
}

// Add an event listener to send the message when the Enter key is pressed
document.getElementById('message').addEventListener('keypress', function (e) {
    if (e.key === 'Enter') {
        sendMessage();
    }
});
