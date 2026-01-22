function grayOut() {
	const dependent = document.getElementById("enable-dependent");
	const enabler = document.getElementById("enabler");
	const soundSelect = document.getElementById("sound-select");
	if (enabler.checked) {
		dependent.style.color = "#000";
		soundSelect.disabled = false;
	} else {
		dependent.style.color = "#777";
		soundSelect.disabled = true;
	}
}