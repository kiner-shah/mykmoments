<script>
    import { loggedInUser } from "$lib/stores.js";
    import { PUBLIC_API_URL } from "$env/static/public";
    import { goto } from "$app/navigation";

    let username;
    let password;

    function validateFormAndTryLogin() {
        if (typeof username !== "string" || typeof password !== "string") {
            return false;
        }
        let url = new URL("/login", PUBLIC_API_URL);
        const sendRequest = () => {
            fetch(url.toString(), {
                method: "POST",
                body: JSON.stringify({username, password})
            }).then(response => {
                if (response.ok) {
                    return response.json();
                }
                throw new Error("Login failed: " + response.statusText);
            }).then(jsonObj => {
                localStorage.setItem("loggedInUser", JSON.stringify(jsonObj));
                loggedInUser.set(jsonObj);
                goto("/dashboard");
            }).catch(error => console.log(error));
        };
        sendRequest();
    }
</script>
<h1>Login</h1>
<section>
    <form id="login-form" method="post">
        <label for="username">Username</label>
        <input name="username" id="username" type="text" bind:value={username} required />

        <label for="password">Password</label>
        <input name="password" id="password" type="password" bind:value={password} required />

        <input type="submit" value="Submit" on:click|preventDefault={validateFormAndTryLogin} />
    </form>
</section>

<style>
    h1 {
        text-align: center;
    }
    section {
        display: flex;
        justify-content: center;
        align-items: flex-start;
        height: 60vh;
        font-size: 1em;
    }
    section form {
        width: 30%;
        display: flex;
        flex-direction: column;
        gap: 5px;
        justify-content: flex-start;
        align-items: flex-start;
    }
    section form input {
        width: 100%;
        height: 30px;
    }
    section form input[type=submit] {
        border-style: none;
        background-color: rgb(250, 229, 107);
        color: rgba(144,128,39,1);
    }
    section form input[type=submit]:hover {
        background-color: rgb(238, 218, 103);
    }
</style>