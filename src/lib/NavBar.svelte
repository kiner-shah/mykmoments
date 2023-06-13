<script>
    import { loggedInUser } from "$lib/stores.js";
    import { PUBLIC_API_URL } from "$env/static/public";
    import { onMount } from "svelte";
    import { goto } from "$app/navigation";

    function onLogoClick() {
        if ($loggedInUser) {
            goto("/dashboard");
        }
        else {
            goto("/");
        }
    }

    let profileMenuVisible = false;
    function toggleProfileMenu() {
        profileMenuVisible = !profileMenuVisible;

        let displayVal = "none";
        if (profileMenuVisible) {
            displayVal = "flex";
        }
        document.getElementById("profile-menu-options").style.display = displayVal;
    }

    function logout() {
        let url = new URL("/logout", PUBLIC_API_URL);
        const sendRequest = () => {
            fetch(url.toString(), {
                method: "POST"
            }).then(response => {
                if (response.ok) {
                    localStorage.removeItem("loggedInUser");
                    loggedInUser.set();
                    location.href = "/";
                }
                throw new Error(response.statusText, {
                    cause: response.status
                });
            }).catch(error => {
                if ("cause" in error) {
                    loginError = error.message;
                }
                else {
                    loginError = "Server is down";
                }
            });
        };
        sendRequest();
    }
</script>

<nav id="navbar">
    <img
        src="/logo1.png"
        alt="logo"
        on:click={onLogoClick}
        on:keydown={onLogoClick}
    />

    {#if !$loggedInUser}
        <section>
            <a href="/createaccount">Create account</a>
            |
            <a href="/login">Login</a>
        </section>
    {:else}
        <section>
            <a href="#" id="profile-menu-expand" on:click|preventDefault={toggleProfileMenu}>{$loggedInUser.username} ▼</a>

            <section id="profile-menu-options">
                <button>Edit profile</button>
                <button>Settings</button>
                <button on:click={logout}>Logout</button>
            </section>
        </section>
    {/if}
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
        box-shadow: 3px 9px 26px -5px rgba(144, 128, 39, 0.49);
        transform: scaleX(2) translateX(20px);
        cursor: pointer;
    }
    #navbar section {
        display: flex;
        flex-direction: row;
        gap: 10px;
        position: relative;
    }
    #navbar section a:link,
    #navbar section a:visited {
        color: rgb(144, 128, 39);
        text-decoration: none;
    }
    #navbar section a:hover {
        text-decoration: underline;
    }
    #navbar section #profile-menu-options {
        position: absolute;
        top: 20px;
        display: none;
        flex-direction: column;
        row-gap: 0%;
        padding: 2%;
        box-shadow: 0px 0px 18px 0px rgba(144, 128, 39, 0.49);
        z-index: 1;
    }
    #navbar section #profile-menu-options button {
        border-style: none;
        width: 100px;
        background-color: white;
        color: rgb(144, 128, 39);
        padding: 10%;
    }
    #navbar section #profile-menu-options button:hover {
        text-decoration: underline;
        cursor: pointer;
    }
</style>