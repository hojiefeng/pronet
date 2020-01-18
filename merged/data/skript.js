const marea = document.getElementById('marea')

const msgRecieved = (msg) => { marea.innerHTML = '[' + Date.now() + '] ' + msg + '<br>' + marea.innerHTML }

/* 

//Connect to websocket
let exampleSocket = new WebSocket("ws://192.168.4.1:81/")

// Sending
exampleSocket.send("Here's some text that the server is urgently awaiting!")

// Receiving
exampleSocket.onmessage = function (event) {
	console.log(event.data);
}
*/
msgRecieved('Waiting for messages...')
