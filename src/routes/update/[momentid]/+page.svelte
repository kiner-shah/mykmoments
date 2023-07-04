<script>
    import { goto } from "$app/navigation";
    import { PUBLIC_API_URL } from "$env/static/public";
    import { loggedInUser } from "$lib/stores.js";
    import FixedStatusMessage from "$lib/FixedStatusMessage.svelte";

    export let data;

    const feelings = ["happy", "sad", "angry", "scared"];

    let description;
    let selected_image;
    const max_description_length = 2000;

    function getDateString(created_date_time) {
        return new Date(created_date_time).toISOString().substring(0,10);
    }

    async function getMoment(momentid) {
        var requestOptions = {
            method: 'GET',
            redirect: 'follow',
            headers: {
                "Authorization": "Bearer " + $loggedInUser.access_token
            }
        };

        const url = new URL("/getmoment", PUBLIC_API_URL);
        const url_search_params = new URLSearchParams({
            'id': momentid
        });
        url.search = url_search_params.toString();

        const response = await fetch(url.toString(), requestOptions);
        if (!response.ok) {
            throw new Error(response.statusText, {cause: response.status});
        }
        const json = await response.json();
        description = json.description;
        return json;
    }

    function handleSubmit() {
        const form = document.getElementById("updatemoment-form");
        const url = new URL("/updatemoment", PUBLIC_API_URL);

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
                    add_moment_response_status.message = "Updated moment successfully";
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
                    add_moment_response_status.message = "Failed to update moment - either server is down or some other error occured";
                }
                // The message will disappear after 10 seconds.
                setTimeout(() => add_moment_response_status = {response_received: false, is_error: false, message: ""}, 10000);
            });
    }

</script>
{#await getMoment(data.momentid)}
    <p id="loading">Loading...</p>
{:then moment}
    <form id="updatemoment-form"  method="post" on:submit|preventDefault={handleSubmit}>
        <label for="moment-title">Title</label>
        <input type="text" id="moment-title" name="moment-title" value={moment.title} />

        <section id="moment-date-section">
            <label for="moment-date">Date</label>
            <input type="date" id="moment-date" name="moment-date" value={moment.date} />
        </section>

        <label for="moment-description">Description</label>
        <textarea name="moment-description" id="moment-description" maxlength="2000" placeholder="Your message (max. 2000 characters)" bind:value={description}></textarea>
        <p id="moment-description-chars-left">Characters left: {max_description_length - description.length}/{max_description_length}</p>

        <label for="moment-feelings">How do you feel?</label>
        <section id="moment-feelings">
            {#each feelings as feeling}
                <input type="checkbox" id={feeling} value={feeling} />
                <label for={feeling}>{feeling[0].toUpperCase() + feeling.slice(1)}</label>
            {/each}
        </section>

        {#if moment.image_filename !== ''}
            <p>Current image associated with this moment: {moment.image_filename}</p>
        {/if}
        <section id="moment-image-section">
            <label for="moment-image">{moment.image_filename !== '' ? 'Change' : 'Add'} image</label>
            <input name="moment-image" id="moment-image" type="file" accept=".png,.jpg" bind:value={selected_image}/>
        </section>

        <label for="moment-image-caption" class:hide-element={moment.image_filename === '' || selected_image === undefined || selected_image === ''}>Image Caption</label>
        <input type="text" id="moment-image-caption" name="moment-image-caption" class:hide-element={moment.image_filename === '' || selected_image === undefined || selected_image === ''} value={moment.image_caption} />

        <section id="form-buttons">
            <button type="submit">Save</button>
            <button type="button" on:click={() => goto("/dashboard")}>Cancel</button>
        </section>
    </form> 
{:catch error}
    <p>{"cause" in error ? error.message : "Failed to retrieve info"}</p>
    <FixedStatusMessage is_error={true} message={"cause" in error ? error.message : "Failed to retrieve info"} />
{/await}

<style>
    form {
        display: flex;
        flex-direction: column;
        width: 60%;
        margin: 4% auto 2% auto;
    }
    #loading {
        text-align: center;
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
    #moment-description {
        height: 50vh;
    }
    #moment-image-section {
        display: block;
    }
    #moment-date-section {
        margin-bottom: 1%;
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
    .hide-element {
        display: none;
    }
</style>