const marea = document.getElementById('marea')

const msgRecieved = (msg) => { marea.innerHTML = '[' + Date.now() + '] ' + msg + '<br>' + marea.innerHTML }

/* 

//Connect to websocket
let exampleSocket = new WebSocket("wss://www.example.com/socketserver")

// Sending
exampleSocket.send("Here's some text that the server is urgently awaiting!")

// Receiving
exampleSocket.onmessage = function (event) {
	console.log(event.data);
}
*/
msgRecieved('Waiting for messages...')