<h1>Create Account</h1>
<section>
    <form action="?/validateAndCreateAccount" method="post">
        <label for="emailid">Email id</label>
        <input name="emailid" id="emailid" type="email" required />

        <label for="username">Username</label>
        <input name="username" id="username" type="text" required />

        <label for="password">Password</label>
        <input name="password" id="password" type="password" required />

        <label for="confirmpassword">Confirm Password</label>
        <input name="confirmpassword" id="confirmpassword" type="password" required />

        <input type="submit" value="Submit"/>
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