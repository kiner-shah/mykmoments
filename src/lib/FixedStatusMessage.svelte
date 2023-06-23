<script>
    export let is_error = false;
    export let message = '';

    let node_reference;

    function destroyComponent() {
        node_reference.parentNode.removeChild(node_reference);
    }

</script>

<section bind:this={node_reference}>
    <span id="message" class:error-message={is_error}>{message}</span>
    <button id="cross-mark" class:error-message={is_error} on:click={destroyComponent}>&times;</button>
</section>

<style>
    section {
        position: fixed;
        top: 2px;
        left: 0px;
        margin-left: auto;
        margin-right: auto;
        width: 100vw;
        display: flex;
        flex-direction: row;
    }
    span, button {
        color: white;
        background-color: darkseagreen;
        padding: 2%;
    }
    #message {
        width: 100%;
    }
    .error-message {
        background-color: darkred;
    }
    #cross-mark {
        font-size: 1.5em;
        cursor: pointer;
        border: none;
    }
</style>