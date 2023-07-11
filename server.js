const express = require('express');
const fileUpload = require('express-fileupload');
const archiver = require('archiver');
const fs = require('fs');
const { exec } = require('child_process');
const path = require('path');

const app = express();
app.use(fileUpload());
const executable_file = 'a.exe';

app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'index.html'));
    fs.access(executable_file, fs.constants.F_OK, (err) => {
        if (err) {
            console.log(`File "${executable_file}" does not exist. Running "make" command...`);

            exec('make', (error, stdout, stderr) => {
                if (error) {
                    console.error('Error running "make" command:', error);
                    return;
                }

                console.log('"make" command executed successfully.');
            });
        } else {
            console.log(`File "${executable_file}" exists.`);
        }
    });
});

app.post('/upload', (req, res) => {
    // function uploadHandler(executable_file) {
    // return (req, res) => {
    const files = req.files && req.files.files;
    const password = req.body.password;
    const mode = req.body.mode;

    // if (!files) { // not needed since already checked for number of files
    //     return res.status(400).send('Please upload a file.');
    // }

    const uploadPath = path.join(__dirname, 'uploads');
    if (!fs.existsSync(uploadPath)) {
        fs.mkdirSync(uploadPath);
    }

    const filePaths = [];

    if (Array.isArray(files)) {
        for (const file of files) {
            const filePath = path.join(uploadPath, file.name);
            filePaths.push(filePath);

            file.mv(filePath, (error) => {
                if (error) {
                    console.error('Error moving file:', error);
                    return res.status(500).send('An error occurred during file movement.');
                }
            });
        }
    } else {
        const filePath = path.join(uploadPath, files.name);
        filePaths.push(filePath);

        files.mv(filePath, (error) => {
            if (error) {
                console.error('Error moving file:', error);
                return res.status(500).send('An error occurred during file movement.');
            }
        });
    }

    // const compileCommand = `g++ ${executable_file}.cpp -o ${executable_file}`;
    // exec(compileCommand, (error, stdout, stderr) => {
    //     if (error) {
    //         console.error(`Error compiling ${executable_file}.cpp:`, error);
    //         return res.status(500).send('An error occurred during compilation.');
    //     }

    const promises = filePaths.map((filePath) => {
        return new Promise((resolve, reject) => {
            const command = `./${executable_file} ${filePath} ${password} ${mode}`;
            exec(command, (error, stdout, stderr) => {
                if (error) {
                    console.error(`Error executing ${executable_file}:`, error);
                    reject(error);
                } else {
                    resolve();
                }
            });
        });
    });

    Promise.all(promises)
        .then(() => {
            const output = fs.createWriteStream('result.zip');
            const archive = archiver('zip', {
                zlib: { level: 9 }
            });

            output.on('close', () => {
                console.log('Result archive created.');
                res.download('result.zip', 'result.zip', (err) => {
                    if (err) {
                        console.error('Error sending result:', err);
                        res.status(500).send('An error occurred during download.');
                    }

                    cleanUp();
                });
            });

            archive.pipe(output);

            if (Array.isArray(files)) {
                for (const file of files) {
                    archive.file(path.join(uploadPath, file.name), { name: file.name });
                }
            } else {
                archive.file(path.join(uploadPath, files.name), { name: files.name });
            }

            archive.finalize();
        })
        .catch((error) => {
            console.error('Error processing files:', error);
            return res.status(500).send('An error occurred during file processing.');
        });
    // });

    function cleanUp() {
        if (Array.isArray(files)) {
            for (const file of files) {
                fs.unlinkSync(path.join(uploadPath, file.name));
            }
        } else {
            fs.unlinkSync(path.join(uploadPath, files.name));
        }

        // fs.unlinkSync(path.join(__dirname, 'result.zip'));
        // fs.unlinkSync(path.join(__dirname, executable_file));
    }

    app.post('/delete', (req, res) => {
        const filename = 'result.zip';
        fs.unlink(path.join(__dirname, filename), (err) => {
            if (err) {
                console.error('Error deleting file:', err);
                return res.status(500).send('An error occurred during file deletion.');
            }
            console.log('File deleted:', filename);
            res.sendStatus(200);
        });
    });
});
// });

// app.post('/enc', uploadHandler('myalgo_a'));
// app.post('/dec', uploadHandler('myalgo_b'));

app.listen(3000, () => {
    console.log('Server is running on port 3000');
});