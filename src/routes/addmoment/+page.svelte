<script>
    import { loggedInUser } from "$lib/stores.js";
    import { goto } from "$app/navigation";
    import { PUBLIC_API_URL } from "$env/static/public";
    import FixedStatusMessage from "$lib/FixedStatusMessage.svelte";

    const max_description_length = 2000;
    const feelings = ["happy", "sad", "angry", "scared"];
    
    let selected_image;
    let description = '';
    let add_moment_response_status = {response_received: false, is_error: false, message: ""};

    function handleSubmit() {
        const form = document.getElementById("addmoment-form");
        const url = new URL("/addmoment", PUBLIC_API_URL);

        const feelings_arr = [];
        document.querySelectorAll("#moment-feelings input").forEach(ele => { if (ele.checked) feelings_arr.push(ele.value); })
        feelings_arr.sort();

        const form_data = new FormData(form);
        if (feelings_arr.length > 0) {
            form_data.append("moment-feelings", feelings_arr.join(','));
        }

        const fetchOptions = {
            method: form.method,
            headers: {
                "Authorization": "Bearer " + $loggedInUser.access_token,
                "Connection": "Keep-Alive"
            },
            body: form_data
        };

        fetch(url.toString(), fetchOptions)
            .then(response => {
                if (response.ok) {
                    add_moment_response_status.is_error = false;
                    add_moment_response_status.message = "Added moment successfully";
                    add_moment_response_status.response_received = true;
                    form.reset();
                    return;
                }
                throw new Error(response.statusText, {
                    cause: response.status
                });
            })
            .catch(error => {
                add_moment_response_status.is_error = true;
                add_moment_response_status.response_received = true;
                if ("cause" in error) {
                    add_moment_response_status.message = error.message;
                }
                else {
                    add_moment_response_status.message = "Failed to add moment - either server is down or some other error occured";
                }
                // The message will disappear after 10 seconds.
                setTimeout(() => add_moment_response_status = {response_received: false, is_error: false, message: ""}, 10000);
            });
    }

</script>

{#if add_moment_response_status.response_received}
<FixedStatusMessage is_error={add_moment_response_status.is_error} message={add_moment_response_status.message} />
{/if}
<form id="addmoment-form" method="post" on:submit|preventDefault={handleSubmit}>
    <label for="moment-title">Title</label>
    <input type="text" id="moment-title" name="moment-title" required />

    <section id="moment-date-section">
        <label for="moment-date">Date</label>
        <input type="date" id="moment-date" name="moment-date" required />
    </section>

    <label for="moment-description">Description</label>
    <textarea name="moment-description" id="moment-description" maxlength="2000" placeholder="Your message (max. 2000 characters)" bind:value={description} required></textarea>
    <p id="moment-description-chars-left">Characters left: {max_description_length - description.length}/{max_description_length}</p>

    <label for="moment-feelings">How do you feel?</label>
    <section id="moment-feelings">
        {#each feelings as feeling}
            <input type="checkbox" id={feeling} value={feeling} />
            <label for={feeling}>{feeling[0].toUpperCase() + feeling.slice(1)}</label>
        {/each}
    </section>

    <section id="moment-image-section">
        <label for="moment-image">Image</label>
        <input name="moment-image" id="moment-image" type="file" accept=".png,.jpg" bind:value={selected_image} />
    </section>

    <!-- Show only if image is added -->
    {#if selected_image !== undefined && selected_image !== ''}
        <label for="moment-image-caption">Image Caption</label>
        <input type="text" id="moment-image-caption" name="moment-image-caption" required />
    {/if}

    <section id="form-buttons">
        <input type="submit" value="Add Moment" />
        <button type="button" on:click={() => goto("/dashboard")}>Cancel</button>
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