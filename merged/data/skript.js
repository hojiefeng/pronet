
const marea = document.getElementById('marea')
const mtxt = document.getElementById('mtxt')

const msgReceived = (msg) => { marea.innerHTML = '[' + Date.now() + '] ' + msg + '<br>' + marea.innerHTML }
const msgSend = () => { 
	const msg = mtxt.value
        exampleSocket.send(msg);
//	msgReceived('<b>You: '+msg+'</b>')
	mtxt.value = ''
}

/*setInterval(() => {
if (exampleSocket.readyState === WebSocket.CLOSED) {
   exampleSocket = new WebSocket("ws://192.168.4.1:81/",['arduino']);
}
}, 1000);*/
 

window.onload = () => {
//Connect to websocket
//exampleSocket = new WebSocket("ws://192.168.4.1:81/",['arduino'])
exampleSocket = new WebSocket("ws://192.168.4.1:81/")
// Sending
//exampleSocket.send("test test")

// Receiving
exampleSocket.onmessage = function (event) {
	msgReceived(event.data);
}

msgReceived('Waiting for messages...')
}
