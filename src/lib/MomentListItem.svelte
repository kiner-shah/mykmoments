<script>
    import { goto } from "$app/navigation";
    
    export let moment;

    let to_delete_moment;
    let delete_confirmation_input_text = '';
    const delete_confirmation_text = "I want to delete this moment";
    let delete_not_allowed = true;
    function confirmAndDeleteMoment(moment) {
        to_delete_moment = moment;
        delete_confirmation_input_text = '';
    }
    function checkDeleteConfirmInputText() {
        delete_not_allowed = true;
        if (delete_confirmation_input_text === delete_confirmation_text) {
            delete_not_allowed = false;
        }
    }

    function convertToHumanReadableDateTimeString(input_date_time) {
        const event = new Date(input_date_time);
        return event.toLocaleString('en-GB', { dateStyle: 'long', timeStyle: 'medium', timeZone: 'IST', hourCycle: 'h12' });
    }

    function getImageData(image_data, image_filename) {
        if (image_data === undefined || image_data.length == 0) {
            return "";
        }
        const file_extension = image_filename.split('.').pop();

        // Thanks to mobz (https://stackoverflow.com/a/9458996)
        // let binary = '';
        // let bytes = new Uint8Array( image_data );
        // var len = bytes.byteLength;
        // for (var i = 0; i < len; i++) {
        //     binary += String.fromCharCode( bytes[ i ] );
        // }

        let binary = "";
        for(let i = 0; i < image_data.length; i++) {
            binary += !(i - 1 & 1) ? String.fromCharCode(parseInt(image_data.substring(i - 1, i + 1), 16)) : ""
        }
        const base64_encoded = btoa( binary );
        return "data:image/" + file_extension + ";base64," + base64_encoded;
    }
</script>

{#if to_delete_moment}
<section id="delete-dialog">
    <form>
        <p><b>You are about to delete the moment titled:</b></p>
        <h3>{to_delete_moment.title}</h3>
        <p><b>Deleting a moment will erase all data associated with this moment and cannot be recovered.</b></p>
        <p><b>Are you sure you want to delete this moment?</b></p>
        <p>Type <q>I want to delete this moment</q> in the below text box to confirm deletion and then click on Delete button</p>

        <input type="text" onpaste="return false" bind:value={delete_confirmation_input_text} on:keyup={() => checkDeleteConfirmInputText()} autocomplete=off />
        <button id="delete-dialog-cancel-button" on:click={() => { to_delete_moment = undefined; }}>Cancel</button>
        <button id="delete-dialog-delete-button" disabled={delete_not_allowed}>Delete</button>
    </form>
</section>
{/if}

<section class="moments-list-item">
    <section class="moments-list-item-image-section">
        <img
            class="moments-list-item-image"
            src={getImageData(moment.image_data, moment.image_filename)}
            alt="moment image"
        />
    </section>
    <section class="moments-list-item-summary">
        <h3>{moment.title}</h3>
        <span>{moment.short_description}</span>
        <section class="moments-list-item-more-info">
            <section class="moments-list-item-meta">
                <span>Date {moment.date}</span> &#8226
                <span>Created {convertToHumanReadableDateTimeString(moment.created_time)}</span> &#8226
                <span>Last modified {convertToHumanReadableDateTimeString(moment.last_modified_time)}</span>
            </section>
            <section class="moments-options">
                <button class="moments-option-item" on:click={() => goto("/show/" + moment.id)}>&#128065</button>
                <button class="moments-option-item" on:click={() => goto("/update/" + moment.id)}>&#128393</button>
                <button class="moments-option-item delete-moment-option" on:click={() => confirmAndDeleteMoment(moment)}>&#128465</button>
            </section>
        </section>
    </section>
</section>

<style>
    button {
        border-style: none;
        height: 30px;
        color: rgba(144, 128, 39, 1);
    }
    .moments-list-item {
        display: flex;
        flex-direction: row;
        column-gap: 10px;
        align-items: center;
        width: 100%;
        border: 1px solid black;
        border-radius: 10px;
        padding: 1%;
        margin-top: 1%;
        background-color: rgba(250, 228, 107, 0.26);
    }
    .moments-list-item-image-section {
        width: 80px;
        height: 100px;
    }
    .moments-list-item-image {
        width: 100%;
        height: 100%;
    }
    .moments-list-item-summary {
        width: 100%;
        overflow: hidden;
        text-overflow: ellipsis;
        white-space: nowrap;
    }
    .moments-list-item-more-info {
        display: flex;
        flex-direction: row;
        justify-content: space-between;
        font-size: 0.75em;
        margin-top: 1%;
        align-items: center;
        width: 100%;
    }
    .moments-options {
        display: flex;
        column-gap: 5px;
        width: 50%;
        justify-content: flex-end;
    }
    .moments-option-item {
        padding: 0%;
        font-size: 2em;
        background-color: inherit;
    }
    .moments-option-item:hover {
        color: black;
        background-color: inherit;
    }
    .delete-moment-option {
        color: darkred;
    }
    .delete-moment-option:hover {
        color: red;
    }

    #delete-dialog {
        position: fixed;
        width: 100%;
        height: 100%;
        top: 0px;
        left: 0px;
        background-color: rgba(0, 0, 0, 0.541);
    }
    #delete-dialog form {
        background-color: white;
        width: 50%;
        margin-top: 50vh;
        transform: translateY(-25vh);
        margin-left: auto;
        margin-right: auto;
        padding: 5%;
        text-align: center;
    }
    #delete-dialog form input[type=text] {
        width: 100%;
        margin-bottom: 2%;
    }
    #delete-dialog-cancel-button {
        border: 1px solid black;
        background-color: white;
        color: black;
    }
    #delete-dialog-cancel-button:hover {
        background-color: rgb(224, 221, 221);
    }
    #delete-dialog-delete-button {
        border: 1px solid red;
        background-color: red;
        color: white;
    }
    #delete-dialog-delete-button:hover,
    #delete-dialog-delete-button:disabled {
        background-color: rgb(192, 1, 1);
        border: 1px solid rgb(192,1,1);
    }
</style>