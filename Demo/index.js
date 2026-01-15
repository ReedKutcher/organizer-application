var tab = "home";

function switchTab(tabName) {
	const content = document.getElementById("content");
	content.src = `${tabName}.html`;
}

function toggleMenu() {
	const menu = document.getElementById("menu");
	const style = document.getElementById("content");
	if (menu.style.width === "160px") {
		menu.style.width = "0";
		content.style.left = "0";
	} else {
		menu.style.width = "160px";
		content.style.left = "160px";
	}
}


const days = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"];
const months = ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"];

function updateTime() {
	let now = new Date();
	let hour = now.getHours();
	let minute = now.getMinutes();
	document.getElementById("banner-date").innerHTML = days[now.getDay()] + ", " + months[now.getMonth()] + " " + now.getDate() + ", " + now.getFullYear();
	document.getElementById("banner-clock").innerHTML = ((hour + 11) % 12 + 1) + ((minute < 10) ? ":0" : ":") + minute + ((hour >= 12) ? " PM" : " AM");
}

updateTime();

setInterval(updateTime, 1000);