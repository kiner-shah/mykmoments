<script>
    import { goto } from "$app/navigation";

    export let data;

    const feelings = ["happy", "sad", "angry", "scared"];

    const max_description_length = 2000;
    function getDateString(created_date_time) {
        return new Date(created_date_time).toISOString().substring(0,10);
    }
</script>

<form>
    <label for="moment-title">Title</label>
    <input type="text" id="moment-title" name="moment-title" value={data.moment.title}>

    <label for="moment-description">Description</label>
    <textarea name="moment-description" id="moment-description" maxlength="2000" placeholder="Your message (max. 2000 characters)" value={data.moment.description}></textarea>
    <p id="moment-description-chars-left">Characters left: 0/{max_description_length}</p>

    <label for="moment-feelings">How do you feel?</label>
    <section id="moment-feelings">
        {#each feelings as feeling}
            <input type="checkbox" id={feeling} value={feeling}>
            <label for={feeling}>{feeling[0].toUpperCase() + feeling.slice(1)}</label>
        {/each}
    </section>

    <p>Current image associated with this moment: {data.moment.image_url}</p>
    <section id="moment-image-section">
        <label for="moment-image">Change image</label>
        <input name="moment-image" id="moment-image" type="file" accept=".png,.jpg">
    </section>

    <section id="moment-date-section">
        <label for="moment-date">Date</label>
        <input type="date" id="moment-date" name="moment-date" value={getDateString(data.moment.created_date_time)}>
    </section>

    <label for="moment-image-caption">Image Caption</label>
    <input type="text" id="moment-image-caption" name="moment-image-caption" value={data.moment.image_caption}>

    <section id="form-buttons">
        <button>Save</button>
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
</style>