var tab = "home";

function switchTab(tabName) {
	const content = document.getElementById("content");
	const banner = document.getElementById("banner");
	const menu = document.getElementById("menu");
	content.src = `${tabName}.html`;
	tab = tabName;
	toggleMenu();
	if (tabName === "calendar") {
		banner.style.minHeight = "12em";
		content.style.top = "12em";
		menu.style.top = "12em";
	} else {
		banner.style.minHeight = "3em";
		content.style.top = "3em";
		menu.style.top = "3em";
	}
}

function toggleMenu() {
	const menu = document.getElementById("menu");
	const style = document.getElementById("content");
	if (menu.style.width === "10em") {
		menu.style.width = "0";
		content.style.left = "5em";
		content.style.right = "5em";
	} else {
		menu.style.width = "10em";
		content.style.left = "10em";
		content.style.right = "0";
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