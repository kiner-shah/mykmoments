<script>
    import { goto } from "$app/navigation";
    import { PUBLIC_API_URL } from "$env/static/public";
    import { loggedInUser } from "$lib/stores.js";
    import FixedStatusMessage from "$lib/FixedStatusMessage.svelte";
    import MomentListItem from "$lib/MomentListItem.svelte";

    async function getTotalMoments() {
        var requestOptions = {
            method: 'GET',
            redirect: 'follow',
            headers: {
                "Authorization": "Bearer " + $loggedInUser.access_token
            }
        };

        const response = await fetch(new URL("/gettotalmoments", PUBLIC_API_URL).toString(), requestOptions);
        if (!response.ok) {
            throw new Error(response.statusText, {cause: response.status});
        }
        const json = await response.json();
        return json["total_moments"];
    }

    let sort_by = 'date-asc';

    const moments_per_page = [10, 20, 50, 100];
    let selected_index_for_moments_per_page = 0;

    let page_range_start = 0;
    let page_range_length = 10;
    let current_page = 1;

    let max_pages = 0;

    let get_moments_list_promise = getMomentList();

    async function getMomentList(search = undefined) {
        var requestOptions = {
            method: 'GET',
            redirect: 'follow',
            headers: {
                "Authorization": "Bearer " + $loggedInUser.access_token
            }
        };

        const url = new URL("/getmomentlist", PUBLIC_API_URL);
        const url_search_params = new URLSearchParams({
            'page_size': moments_per_page[selected_index_for_moments_per_page],
            'current_page': current_page,
            'sort_by': sort_by
        });
        if (search !== undefined) {
            url_search_params.append("search", search);
        }
        url.search = url_search_params.toString();

        const response = await fetch(url.toString(), requestOptions);
        if (!response.ok) {
            throw new Error(response.statusText, {cause: response.status});
        }
        const json = await response.json();
        max_pages = Math.ceil(json["moments"].length / moments_per_page[selected_index_for_moments_per_page]);
        page_range_length = Math.min(max_pages, page_range_length);
        return json["moments"];
    }

    function changeToPage(page_number) {
        if (page_number <= 0 || page_number > max_pages) {
            return;
        }
        current_page = page_number;
        if (current_page < page_range_start + 1) {
            page_range_start = current_page - 1;
        } else if (current_page > page_range_start + page_range_length) {
            page_range_start = current_page - page_range_length;
        }
        console.log(
            "Current page is now: " +
                current_page +
                " page range start: " +
                page_range_start
        );
        get_moments_list_promise = getMomentList();
    }
</script>

<svelte:head>
    <title>Dashboard</title>
</svelte:head>

<section id="moments-header">
    <section id="moments-summary">
    {#await getTotalMoments()}
        <h3>Total moments: Retrieving...</h3>
    {:then total_moments}
        <h3>Total moments: {total_moments}</h3>
    {:catch error}
        <h3>Total moments: error occured</h3>
        <FixedStatusMessage is_error={true} message={"cause" in error ? error.message : "Failed to retrieve info"} />
    {/await}
        <button id="add-new-moment" on:click={() => goto("/addmoment")}>Add New Moment</button>
    </section>
    <section id="moments-search-bar-and-sort">
        <input type="search" id="searchbar" placeholder="Search title" />
        <button on:click={() => get_moments_list_promise = getMomentList(document.getElementById('searchbar').value)}>Search</button>
        <section id="moments-sort-by">
            <label for="sort-by">Sort by</label>
            <select id="sort-by" name="sort-by" bind:value={sort_by} on:change={() => get_moments_list_promise = getMomentList()}>
                <option value="date-asc">Date posted (ascending)</option>
                <option value="date-desc">Date posted (descending)</option>
            </select>
        </section>
    </section>
</section>

<section id="moments-list">
    {#await get_moments_list_promise}
        Retrieving data...
    {:then moments_data}
        {#if moments_data.length == 0}
            No data found
        {:else}
            {#each moments_data as moment}
                <MomentListItem moment={moment} />
            {/each}
        {/if}
    {:catch error}
        <FixedStatusMessage is_error={true} message={"cause" in error ? error.message : "Failed to retrieve info"} />
    {/await}
</section>

<section id="moments-footer">
    <section id="moments-page">
        <button
            disabled={current_page == 1}
            on:click={() => changeToPage(current_page - 1)}>Prev</button
        >
        <!-- display 10 page buttons at a time -->
        {#each { length: page_range_length } as _, i}
            <button
                on:click={() => changeToPage(page_range_start + i + 1)}
                class:active-page={current_page == page_range_start + i + 1}
                >{page_range_start + i + 1}</button
            >
        {/each}
        <button
            disabled={current_page == max_pages}
            on:click={() => changeToPage(current_page + 1)}>Next</button
        >
    </section>

    <section id="moments-per-page">
        <span>Moments per page:&nbsp;</span>
        {#each moments_per_page as moment_per_page, index}
            <button
                on:click={() => {
                    selected_index_for_moments_per_page = index;
                    get_moments_list_promise = getMomentList();
                }}
                class:moments-per-page-active={index ===
                    selected_index_for_moments_per_page}
                >{moment_per_page}</button
            >
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
        color: rgba(144, 128, 39, 1);
        padding: 2%;
    }
    button:hover,
    button:disabled {
        background-color: rgb(238, 218, 103);
    }
    .moments-per-page-active {
        background-color: rgb(238, 218, 103);
    }
    .active-page {
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
        width: 50%;
    }
    #moments-per-page {
        display: flex;
        justify-content: flex-end;
        align-items: center;
        width: 50%;
    }

    #moments-list {
        width: 80%;
        margin: 0px auto 2% auto;
    }
</style>
