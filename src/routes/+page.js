/*
export async function load({fetch}) {
    return fetch('http://localhost:18080/image', {
        mode: 'cors'
    }).then(result => {
        console.log(result);
        if (result.ok) {
            return {status: result.status, body: result.body};
        }
        throw new Error("Request failed");
    }).catch(error => {
        return {status: 500, body: error.toString()};
    });
}*/