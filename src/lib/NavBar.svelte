<script>
    function onLogoClick(event) {
        window.location.href = "/";
    }
</script>

<nav id="navbar">
    <img src="logo1.png" alt="logo" on:click={onLogoClick} on:keydown={onLogoClick}/>

    <section>
        <a href="/createaccount">Create account</a>
        |
        <a href="/login">Login</a>
    </section>
</nav>

<style>
    #navbar {
        display: flex;
        align-items: center;
        justify-content: space-between;
        background-color: rgb(250, 229, 107);
        color: rgb(144, 128, 39);
        padding: 1% 3% 1% 3%;
    }
    #navbar img {
        width: 200px;
        height: 100px;
        background-color: white;
        box-shadow: 3px 9px 26px -5px rgba(144,128,39,0.49);
        transform: scaleX(2) translateX(20px);
        cursor: pointer;
    }
    #navbar section {
        display: flex;
        flex-direction: row;
        gap: 10px;
    }
    #navbar section a:link,
    #navbar section a:visited {
        color: rgb(144, 128, 39);
        text-decoration: none;
    }
    #navbar section a:hover {
        text-decoration: underline;
    }
</style>