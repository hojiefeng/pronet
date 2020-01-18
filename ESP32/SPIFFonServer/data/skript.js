button = document.getElementById('datebt').addEventListener('click', dateButton)
console.log('hiii')

function dateButton() {
	console.log('hai')
	document.getElementById('demo').innerHTML = Date()
}
