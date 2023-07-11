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
    // var folderUpload = document.getElementById("folder-upload");

    // if (!fileUpload.files.length && !folderUpload.files.length) {
    if (!fileUpload.files.length) {
        showErrorPopup();
        return;
    }

    document.getElementById("popup_cover").style.display = "flex";
    document.getElementById("encrypt-popup").style.display = "flex";
}

function showDecryptPopup() {
    var fileUpload = document.getElementById("file-upload");
    // var folderUpload = document.getElementById("folder-upload");

    // if (!fileUpload.files.length && !folderUpload.files.length) {
    if (!fileUpload.files.length) {
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
    sendToBackend(password, 1);
    document.getElementById("popup_cover").style.display = "none";
    document.getElementById("encrypt-popup").style.display = "none";
}

function submitDecryption() {
    var password = document.getElementById("decrypt-password").value;
    sendToBackend(password, 2);
    document.getElementById("popup_cover").style.display = "none";
    document.getElementById("decrypt-popup").style.display = "none";
}

function sendToBackend(password, mode) {
    document.getElementById("processing").style.display = "flex";

    const form = document.getElementById('uploadForm');
    const fileInput = document.getElementById('file-upload');
    const formData = new FormData(form);

    formData.append('password', password);
    formData.append('mode', mode);

    fetch('/upload', {
        method: 'POST',
        body: formData
    })
        .then(response => {
            if (!response.ok) {
                throw new Error('Upload failed.');
            }
            return response.blob();
        })
        .then(blob => {
            const downloadUrl = URL.createObjectURL(blob);
            const link = document.createElement('a');
            link.href = downloadUrl;
            link.download = 'result.zip';
            link.click();
            URL.revokeObjectURL(downloadUrl);

            // Reset the form
            fileInput.value = null;


            // Delete the result.zip file
            fetch('/delete', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                }
            })
                .then(response => {
                    if (!response.ok) {
                        throw new Error('Error deleting file.');
                    }
                })
                .catch(error => {
                    console.error('Error:', error);
                });
        })
        .catch(error => {
            console.error('Error:', error);
        });

    setTimeout(function () {
        document.getElementById("processing").style.display = "none";
    }, 2000);
}