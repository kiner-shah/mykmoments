<script>

</script>

<svelte:head>
    <title>Dashboard</title>
</svelte:head>

<section id="moments-header">
    <input type="search" id="searchbar" placeholder="Search title">
    <section>
        <label for="sort-by">Sort by</label>
        <select id="sort-by" name="sort-by">
            <option value="date-asc">Date posted (ascending)</option>
            <option value="date-desc">Date posted (descending)</option>
        </select>
    </section>
</section>

<style>
    #moments-header {
        display: flex;
        width: 80%;
        flex-direction: row;
        justify-content: space-between;
        margin-left: auto;
        margin-right: auto;
        column-gap: 20px;
        align-items: center;
    }
    #searchbar {
        height: 50px;
        width: 100%;
    }
</style>