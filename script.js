function showErrorPopup() {
    document.getElementById("error-popup").style.display = "block";
}

function closeErrorPopup() {
    document.getElementById("error-popup").style.display = "none";
}

function showEncryptPopup() {
    var fileUpload = document.getElementById("file-upload");
    var folderUpload = document.getElementById("folder-upload");

    if (!fileUpload.files.length && !folderUpload.files.length) {
        showErrorPopup();
        return;
    }

    document.getElementById("encrypt-popup").style.display = "block";
}

function showDecryptPopup() {
    var fileUpload = document.getElementById("file-upload");
    var folderUpload = document.getElementById("folder-upload");

    if (!fileUpload.files.length && !folderUpload.files.length) {
        showErrorPopup();
        return;
    }

    document.getElementById("decrypt-popup").style.display = "block";
}

function checkPasswordStrength(password) {
    var passwordStrength = document.getElementById("password-strength");
    var segments = Array.from(passwordStrength.children);

    segments.forEach(function (segment, index) {
        if (index < password.length) {
            segment.style.backgroundColor = getColor(index);
        } else {
            segment.style.backgroundColor = "";
        }
    });
}

function getColor(index) {
    var gradient = Math.round((index / 8) * 255);
    return "rgb(" + gradient + ", " + (255 - gradient) + ", 0)";
}

function checkPasswordMatch() {
    var password = document.getElementById("password").value;
    var confirmPassword = document.getElementById("confirm-password").value;
    var passwordMatch = document.getElementById("password-match");

    if (password !== confirmPassword) {
        passwordMatch.style.display = "block";
    } else {
        passwordMatch.style.display = "none";
        document.getElementById("submit-button").style.display = "block";
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
    document.getElementById("processing").style.display = "block";

    setTimeout(function () {
        document.getElementById("processing").style.display = "none";
        showDownloadButton();
    }, 2000);
}

function showDownloadButton() {
    var downloadButton = document.createElement("a");
    downloadButton.setAttribute("href", "#");
    downloadButton.setAttribute("download", "encrypted_files.zip");
    downloadButton.innerText = "Download";
    downloadButton.classList.add("download-button");

    var container = document.getElementsByClassName("container")[0];
    container.appendChild(downloadButton);
}  