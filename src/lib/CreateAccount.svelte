<script>
    import { PUBLIC_API_URL } from "$env/static/public";
    import { goto } from "$app/navigation";

    let createAccountError;

    function validateFormAndCreateAccount() {
        const form = document.getElementById("createaccount-form");

        const form_data = new FormData(form);
        const password = form_data.get("password");
        const confirmpassword = form_data.get("confirmpassword");

        if (password !== confirmpassword) {
            createAccountError = "Passwords don't match";
            setTimeout(() => createAccountError = undefined, 10000);
            return;
        }
        form_data.delete("confirmpassword");

        let url = new URL("/createaccount", PUBLIC_API_URL);
        const sendRequest = () => {
            fetch(url.toString(), {
                method: form.method,
                body: form_data
            }).then(response => {
                if (response.ok) {
                    return response.json();
                }
                throw new Error(response.statusText, {
                    cause: response.status
                });
            }).then(jsonObj => {
                // sessionStorage.setItem("loggedInUser", JSON.stringify(jsonObj));
                // loggedInUser.set(jsonObj);
                // goto("/dashboard");
            }).catch(error => {
                if ("cause" in error) {
                    createAccountError = error.message;
                }
                else {
                    createAccountError = "Failed to create account - either server is down or some other error occured";
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
    <form id="createaccount-form" method="post" on:submit|preventDefault={validateFormAndCreateAccount}>
        {#if createAccountError !== undefined}
        <p id="error-message" bind:innerHTML={createAccountError} contenteditable="false"></p>
        {/if}

        <label for="fullname">Full name</label>
        <input name="fullname" id="fullname" type="text" required />

        <label for="birthdate">Birth date</label>
        <input name="birthdate" id="birthdate" type="date" required />

        <label for="emailid">Email id</label>
        <input name="emailid" id="emailid" type="email" required />

        <label for="username">Username</label>
        <input name="username" id="username" type="text" required />

        <label for="password">Password</label>
        <input name="password" id="password" type="password" required />

        <label for="confirmpassword">Confirm Password</label>
        <input name="confirmpassword" id="confirmpassword" type="password" required />

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
        min-height: 60vh;
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
        margin-bottom: 2%;
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