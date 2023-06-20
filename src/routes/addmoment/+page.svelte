<script>
    import { loggedInUser } from "$lib/stores.js";
    import { goto } from "$app/navigation";
    import { PUBLIC_API_URL } from "$env/static/public";

    const max_description_length = 2000;
    const feelings = ["happy", "sad", "angry", "scared"];

    function handleSubmit() {
        const form = document.getElementById("addmoment-form");
        const url = new URL("/addmoment", PUBLIC_API_URL);

        const fetchOptions = {
            method: form.method,
            headers: {
                "Authorization": "Bearer " + $loggedInUser.access_token,
                "Connection": "Keep-Alive"
            },
            body: new FormData(form)
        };

        fetch(url.toString(), fetchOptions)
            .then(response => console.log(response))
            .catch(error => console.log(error));
    }

</script>

<form id="addmoment-form" method="post" enctype="multipart/form-data">
    <label for="moment-title">Title</label>
    <input type="text" id="moment-title" name="moment-title" required>

    <label for="moment-description">Description</label>
    <textarea name="moment-description" id="moment-description" maxlength="2000" placeholder="Your message (max. 2000 characters)" required></textarea>
    <p id="moment-description-chars-left">Characters left: 0/{max_description_length}</p>

    <label for="moment-feelings">How do you feel?</label>
    <section id="moment-feelings">
        {#each feelings as feeling}
            <input type="checkbox" id={feeling} value={feeling}>
            <label for={feeling}>{feeling[0].toUpperCase() + feeling.slice(1)}</label>
        {/each}
    </section>

    <section id="moment-date-section">
        <label for="moment-date">Date</label>
        <input type="date" id="moment-date" name="moment-date" required>
    </section>

    <section id="moment-image-section">
        <label for="moment-image">Image</label>
        <input name="moment-image" id="moment-image" type="file" accept=".png,.jpg" required>
    </section>

    <!-- Show only if image is added -->
    <label for="moment-image-caption">Image Caption</label>
    <input type="text" id="moment-image-caption" name="moment-image-caption" required>

    <section id="form-buttons">
        <input type="submit" value="Add Moment" on:click|preventDefault={handleSubmit}>
        <button on:click={() => goto("/dashboard")}>Cancel</button>
    </section>
</form>

<style>
    form {
        display: flex;
        flex-direction: column;
        width: 60%;
        margin: 4% auto 2% auto;
    }
    input,
    button {
        height: 30px;
    }
    input[type=submit],
    button {
        border-style: none;
        background-color: rgb(250, 229, 107);
        color: rgba(144,128,39,1);
    }
    input[type=submit]:hover,
    button:hover {
        background-color: rgb(238, 218, 103);
    }
    #moment-description {
        height: 50vh;
    }
    #moment-date-section {
        margin-bottom: 1%;
    }
    #moment-image-section {
        display: block;
    }
    #moment-description-chars-left {
        margin-top: 0%;
        text-align: right;
    }
    #moment-feelings {
        display: flex;
        flex-direction: row;
        align-items: center;
    }
    #form-buttons {
        display: flex;
        flex-direction: row;
        column-gap: 10px;
        margin-top: 2%;
    }
</style>