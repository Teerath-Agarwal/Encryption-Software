const express = require('express');
const fileUpload = require('express-fileupload');
const archiver = require('archiver');
const fs = require('fs');
const { exec } = require('child_process');
const path = require('path');

const port = 3000;
const app = express();
app.use(fileUpload());
app.use(express.static('public'));
const executable_file = 'a.exe';

// make command to be added in built commands

// app.get('/', (req, res) => {
//     fs.access(executable_file, fs.constants.F_OK, (err) => {
//         if (err) {
//             console.log(`File "${executable_file}" does not exist. Running "make" command...`);

//             exec('make', (error, stdout, stderr) => {
//                 if (error) {
//                     console.error('Error running "make" command:', error);
//                     return;
//                 }

//                 console.log('"make" command executed successfully.');
//             });
//         }
//     });
// });

app.post('/upload', (req, res) => {
    const files = req.files && req.files.files;
    const password = req.body.password;
    const mode = req.body.mode;

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

    const promises = filePaths.map((filePath) => {
        return new Promise((resolve, reject) => {
            const command = `./${executable_file} ${filePath} ${password} ${mode}`;

            const childProcess = exec(command, (error, stdout, stderr) => {
                if (error) {
                    console.error(`Error executing ${executable_file}:`, error);
                    reject(error);
                    // const code = error.code;
                    // if (code === 1) {
                    //     return res.status(300).send('src error!');
                    // }
                    // else if (code === 2) {
                    //     return res.status(400).send('src error!');
                    // }
                    // else {
                    //     return res.status(600).send('src error!');
                    // }
                } else {
                    resolve();
                }
            });

            // childProcess.on('close', (code) => {
            //     console.log(`Child process exited with code: ${code}`);
            //     if (code !== 0) {
            //     }
            // });
        });
    });

    Promise.all(promises)
        .then(() => {
            const output = fs.createWriteStream('result.zip');
            const archive = archiver('zip', {
                zlib: { level: 9 }
            });

            output.on('close', () => {
                // console.log('Result archive created.');
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

    function cleanUp() {
        const deleteFolderRecursive = (folderPath) => {
            if (fs.existsSync(folderPath)) {
                fs.readdirSync(folderPath).forEach((file) => {
                    const curPath = path.join(folderPath, file);

                    if (fs.lstatSync(curPath).isDirectory()) {
                        deleteFolderRecursive(curPath);
                    } else {
                        fs.unlinkSync(curPath);
                    }
                });

                fs.rmdirSync(folderPath);
                // console.log(`Folder "${folderPath}" deleted successfully.`);
            } else {
                console.log(`Folder "${folderPath}" does not exist.`);
            }
        };
        deleteFolderRecursive(uploadPath);;
    }

});

app.post('/delete', (req, res) => {
    const filename = 'result.zip';
    fs.unlink(path.join(__dirname, filename), (err) => {
        if (err) {
            console.log('Error deleting file:', err);
            return res.status(500).send('An error occurred during file deletion.');
        }
        // console.log('File deleted:', filename);
        res.sendStatus(200);
    });
});

app.listen(port, () => {
    console.log('Server is running on port ' + port);
});
// module.exports = app;
