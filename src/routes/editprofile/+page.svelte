<script>
    import { goto } from "$app/navigation";
</script>
<svelte:head>
    <title>Edit profile</title>
</svelte:head>

<h1>Edit profile</h1>

<form>
    <label for="full-name">Full name</label>
    <input id="full-name" name="full-name" type="text" value="Kiner Shah">
    
    <label for="birth-date">Birth date</label>
    <input id="birth-date" name="birth-date" type="date" value="2023-06-20">

    <section id="form-buttons">
        <input type="submit" value="Save" on:click|preventDefault={() => console.log("submit")}>
        <button type="button" on:click={() => goto("/dashboard") }>Cancel</button>
    </section>
</form>

<style>
    h1 {
        text-align: center;
    }
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
    #form-buttons {
        display: flex;
        flex-direction: row;
        column-gap: 10px;
        margin-top: 2%;
    }
</style>