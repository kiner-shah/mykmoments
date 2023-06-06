<script>
    let moment_count = 0;

    let moments_per_page = [10, 20, 50, 100];
    let selected_moments_per_page = 0;
</script>

<svelte:head>
    <title>Dashboard</title>
</svelte:head>

<section id="moments-header">
    <section id="moments-summary">
        <h3>Total moments: {moment_count}</h3>
        <button id="add-new-moment">Add New Moment</button>
    </section>
    <section id="moments-search-bar-and-sort">
        <input type="search" id="searchbar" placeholder="Search title">
        <section id="moments-sort-by">
            <label for="sort-by">Sort by</label>
            <select id="sort-by" name="sort-by">
                <option value="date-asc">Date posted (ascending)</option>
                <option value="date-desc">Date posted (descending)</option>
            </select>
        </section>
    </section>
</section>

<section id="moments-footer">
    <section id="moments-page">
        <button>Prev</button>
        <button>1</button>
        <button>2</button>
        <button>3</button>
        <button>4</button>
        <button>5</button>
        <button>6</button>
        <button>Next</button>
    </section>

    <section id="moments-per-page">
        <span>Moments per page: &nbsp;</span>
        {#each moments_per_page as moment_per_page, index}
        <button on:click={() => {selected_moments_per_page = index;}} class:moments-per-page-active={index === selected_moments_per_page}>{moment_per_page}</button>
        {/each}
    </section>
</section>

<style>
    #moments-summary,
    #moments-search-bar-and-sort {
        display: flex;
        width: 80%;
        flex-direction: row;
        justify-content: space-between;
        margin: 1% auto 1% auto;
        column-gap: 20px;
        align-items: center;
    }
    #searchbar {
        height: 50px;
        width: 100%;
    }
    button {
        border-style: none;
        background-color: rgb(250, 229, 107);
        color: rgba(144,128,39,1);
        padding: 2%;
    }
    button:hover {
        background-color: rgb(238, 218, 103);
    }
    .moments-per-page-active {
        background-color: rgb(238, 218, 103);
    }
    #moments-footer {
        width: 80%;
        display: flex;
        flex-direction: row;
        margin-bottom: 1%;
        margin-left: auto;
        margin-right: auto;
    }
    #moments-page {
        display: flex;
        width: 50%
    }
    #moments-per-page {
        display: flex;
        justify-content: flex-end;
        align-items: center;
        width: 50%
    }
</style>