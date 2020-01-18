const marea = document.getElementById('marea')
const mtxt = document.getElementById('mtxt')

const msgRecieved = (msg) => { marea.innerHTML = '[' + Date.now() + '] ' + msg + '<br>' + marea.innerHTML }
const msgSend = () => { 
	const msg = mtxt.value
	msgRecieved('<b>You: '+msg+'</b>')
	mtxt.value = ''
}

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