<script>
    import { goto } from "$app/navigation";

    const max_description_length = 2000;
    
</script>

<form>
    <label for="moment-title">Title</label>
    <input type="text" id="moment-title" name="moment-title" required>

    <label for="moment-description">Description</label>
    <textarea name="moment-description" id="moment-description" maxlength="2000" placeholder="Your message (max. 2000 characters)" required></textarea>
    <p id="moment-description-chars-left">Characters left: 0/{max_description_length}</p>

    <label for="moment-feelings">How do you feel?</label>
    <section id="moment-feelings">
        <input type="checkbox" id="happy">
        <label for="happy">Happy</label>
        <input type="checkbox" id="sad">
        <label for="sad">Sad</label>
    </section>

    <section id="moment-image-section">
        <label for="moment-image">Image</label>
        <input name="moment-image" id="moment-image" type="file" accept=".png,.jpg">
    </section>

    <!-- Show only if image is added -->
    <label for="moment-image-caption">Image Caption</label>
    <input type="text" id="moment-image-caption" name="moment-image-caption" required>

    <section id="form-buttons">
        <button>Add Moment</button>
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
    #moment-description {
        height: 50vh;
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