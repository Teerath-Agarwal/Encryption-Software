const ps_bars = document.querySelectorAll('span');

function showErrorPopup() {
    document.getElementById("error-popup").style.display = "flex";
    document.getElementById("popup_cover").style.display = "flex";
}

function closeErrorPopup() {
    document.getElementById("popup_cover").style.display = "none";
    document.getElementById("error-popup").style.display = "none";
}

function showEncryptPopup() {
    var fileUpload = document.getElementById("file-upload");
    var folderUpload = document.getElementById("folder-upload");

    if (!fileUpload.files.length && !folderUpload.files.length) {
        showErrorPopup();
        return;
    }

    document.getElementById("popup_cover").style.display = "flex";
    document.getElementById("encrypt-popup").style.display = "flex";
}

function showDecryptPopup() {
    var fileUpload = document.getElementById("file-upload");
    var folderUpload = document.getElementById("folder-upload");

    if (!fileUpload.files.length && !folderUpload.files.length) {
        showErrorPopup();
        return;
    }

    document.getElementById("popup_cover").style.display = "flex";
    document.getElementById("decrypt-popup").style.display = "flex";
}

function checkPasswordStrength(password) {
    let color = password.length < 4 ? "#FF0000" : (password.length < 6 ? "yellow" : "#00FF00");    
    ps_bars.forEach((segment, index) => {
        segment.style.backgroundColor = index < password.length ? color : "rgb(0, 0, 0, 0)";
    });
}

function checkPasswordMatch() {
    var password = document.getElementById("password").value;
    var confirmPassword = document.getElementById("confirm-password").value;
    var passwordMatch = document.getElementById("password-match");

    if (password !== confirmPassword) {
        passwordMatch.style.display = "flex";
    } else {
        passwordMatch.style.display = "none";
        document.getElementById("submit-button").style.display = "flex";
    }
}

function submitEncryption() {
    var password = document.getElementById("confirm-password").value;
    sendToBackend(password);
}

function submitDecryption() {
    var password = document.getElementById("decrypt-password").value;
    sendToBackend(password);
}

function sendToBackend(password) {
    document.getElementById("processing").style.display = "flex";

    setTimeout(function () {
        document.getElementById("processing").style.display = "none";
        showDownloadButton();
    }, 2000);
}

// function showDownloadButton() {
//     var downloadButton = document.createElement("a");
//     downloadButton.setAttribute("href", "#");
//     downloadButton.setAttribute("download", "encrypted_files.zip");
//     downloadButton.innerText = "Download";
//     downloadButton.classList.add("download-button");

//     var container = document.getElementsByClassName("container")[0];
//     container.appendChild(downloadButton);
// }