const readline = require('readline');
const fetch = require('node-fetch');

const serverAddress = 'http://localhost:3000';
const defaultPrompt = '> ';

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

let connection = {
    isConnected: false,
    computerId: null,
    computerName: null,
    connect(id, name) {
        this.isConnected = true;
        this.computerId = id;
        this.computerName = name;
    },

    disconnect() {
        this.isConnected = false;
        this.computerId = null;
        this.computerName = null;
    }
};

rl.setPrompt(defaultPrompt);
rl.prompt();
rl.on('line', input => {
    if (input === "exit") rl.close();
    handleCommand(input).then(message => {
        console.log(message);
        rl.prompt();
    });
}).on('close',() => {
    process.exit(0);
});

function handleCommand(command) {
    if (command.startsWith('connect ')) {
        let [, computerId] = command.split(' ');
        if (computerId) {
            return fetch(`${serverAddress}/connect?id=${computerId}`).then(res => {
                if (res.ok) {
                    return res.text().then(computerName => {
                        connection.connect(computerId, computerName);

                        rl.setPrompt(`${computerName}> `);
                    }).then(() => "Successfully connected");
                } else {
                    return res.text();
                }
            }, err => 'Error while connecting to server');
        } else {
            return new Promise(resolve => {
                resolve('Use "connect <id>" for connection');
            });
        }
    } else if (command.startsWith('disconnect')) {
        return new Promise(resolve => {
            connection.disconnect();
            rl.setPrompt(defaultPrompt);
            resolve('Disconnected');
        });
    } else {
        if (!connection.isConnected) {
            return new Promise(resolve => {
                resolve('You must connect first');
            });
        }

        return fetch(`${serverAddress}?id=${connection.computerId}&command=${command}`).then(res => {
            return res.text();
        }, err => 'Error while connecting to server');
    }
}