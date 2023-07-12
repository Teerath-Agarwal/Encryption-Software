const ps_bars = document.querySelectorAll('span');
const e0 = 'Please upload a file before proceeding!';
const e1 = "The file name(s) contains spaces";
const e2 = "Incorrect password!";
const e3 = "Unknown error occurred!!";
const pi = document.querySelectorAll('.password-input');

function showErrorPopup(error) {
    document.getElementById("error-txt").innerText = error;
    document.getElementById("error-popup").style.display = "flex";
    document.getElementById("popup_cover").style.display = "flex";
}

function closeErrorPopup() {
    document.getElementById("popup_cover").style.display = "none";
    document.getElementById("error-popup").style.display = "none";
}

function showEncryptPopup() {
    var fileUpload = document.getElementById("file-upload");
    if (!fileUpload.files.length) {
        showErrorPopup(e0);
        return;
    }

    document.getElementById("popup_cover").style.display = "flex";
    document.getElementById("encrypt-popup").style.display = "flex";
}

function showDecryptPopup() {
    var fileUpload = document.getElementById("file-upload");
    if (!fileUpload.files.length) {
        showErrorPopup(e0);
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
}

function submitDecryption() {
    var password = document.getElementById("decrypt-password").value;
    sendToBackend(password, 2);
}

function sendToBackend(password, mode) {
    pi.forEach((inputbox) => inputbox.value = '');
    ps_bars.forEach((segment) => {
        segment.style.backgroundColor = "rgb(0, 0, 0, 0)";
    });
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
            fileInput.value = null;
            if (!response.ok) {
                // if (response.status == 300) {
                //     showErrorPopup(e1);
                //     // throw new Error(e1);
                // }
                // else if (response.status == 400) {
                //     showErrorPopup(e2);
                //     // throw new Error(e2);
                // }
                // else if (response.status == 600) {
                //     showErrorPopup(e3);
                //     // throw new Error(e3);
                // }
                // else if (response.status == 500) {
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

            document.getElementById("processing").style.display = "none";
            document.getElementById("popup_cover").style.display = "none";
            if (mode == 1)
                document.getElementById("encrypt-popup").style.display = "none";
            else
                document.getElementById("decrypt-popup").style.display = "none";
        })
        .catch(error => {
            console.error('Error:', error);
            document.getElementById("processing").style.display = "none";
            document.getElementById("popup_cover").style.display = "none";
            if (mode == 1)
                document.getElementById("encrypt-popup").style.display = "none";
            else
                document.getElementById("decrypt-popup").style.display = "none";
            alert("Error: Ensure the following:\n1.) The file name does not contain spaces\n2.) The file name has some non empty extension\n3.) Password  must be correct")
        });
}