<script>
    import { PUBLIC_API_URL } from "$env/static/public";
    import { goto } from "$app/navigation";

    let emailid;
    let username;
    let password;
    let confirmpassword;
    let createAccountError;

    function validateFormAndCreateAccount() {
        if (typeof emailid !== "string" || typeof username !== "string" || typeof password !== "string" || typeof confirmpassword !== "string") {
            return false;
        }
        if (password !== confirmpassword) {
            createAccountError = "Passwords don't match";
            setTimeout(() => createAccountError = undefined, 10000);
            return;
        }
        let url = new URL("/createaccount", PUBLIC_API_URL);
        const sendRequest = () => {
            fetch(url.toString(), {
                method: "POST",
                body: JSON.stringify({emailid, username, password})
            }).then(response => {
                if (response.ok) {
                    return response.json();
                }
                throw new Error(response.statusText, {
                    cause: response.status
                });
            }).then(jsonObj => {
                // localStorage.setItem("loggedInUser", JSON.stringify(jsonObj));
                // loggedInUser.set(jsonObj);
                // goto("/dashboard");
            }).catch(error => {
                if ("cause" in error) {
                    createAccountError = error.message;
                }
                else {
                    createAccountError = "Server is down";
                }
                // The message will disappear after 10 seconds.
                setTimeout(() => createAccountError = undefined, 10000);
            });
        };
        sendRequest();
    }
</script>
<h1>Create Account</h1>
<section>
    <form method="post" on:submit|preventDefault={validateFormAndCreateAccount}>
        {#if createAccountError !== undefined}
        <p id="error-message" bind:innerHTML={createAccountError} contenteditable="false"></p>
        {/if}
        <label for="emailid">Email id</label>
        <input name="emailid" id="emailid" type="email" bind:value={emailid} required />

        <label for="username">Username</label>
        <input name="username" id="username" type="text" bind:value={username} required />

        <label for="password">Password</label>
        <input name="password" id="password" type="password" bind:value={password} required />

        <label for="confirmpassword">Confirm Password</label>
        <input name="confirmpassword" id="confirmpassword" type="password" bind:value={confirmpassword} required />

        <input type="submit" value="Submit" />
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
    #error-message {
        background-color: rgb(133, 48, 48);
        color: white;
        width: 100%;
        border: none;
        text-align: center;
        padding: 1%;
    }
</style>