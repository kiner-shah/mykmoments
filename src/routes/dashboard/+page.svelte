<script>
    import { goto } from "$app/navigation";
    // 20 December 2012 at 08:30:00 pm
    // const event = new Date(Date.UTC(2012, 11, 20, 15, 0, 0));
    // console.log(event.toLocaleString('en-GB', { dateStyle: 'long', timeStyle: 'medium', timeZone: 'IST', hour12: 'true' }));

    // TODO: replace with data fetched from backend
    let moment_count = 2;

    let moments_per_page = [10, 20, 50, 100];
    let selected_index_for_moments_per_page = 0;

    let page_range_start = 0;
    const page_range_length = 10;
    let current_page = 1;
    // TODO: replace with data fetched from backend
    let max_pages = 100;

    // TODO: replace with data fetched from backend
    let moments_data = [
        {
            id: "moment0",
            title: "Title 0",
            description:
                "This is a very long summary for this moment. I have no idea what to do to get this summary, so I had no choice but to use this dummy",
            created_by_username: "user1",
            created_date_time: "9 Jun 2023 3:16:33 pm",
            last_edit_date_time: "9 Jun 2023 3:16:33 pm",
            image_url: "https://picsum.photos/200",
            image_caption: "This is a sample caption for image",
        },
        {
            id: "moment1",
            title: "Title 1",
            description:
                "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam sed lorem vehicula, pellentesque dolor quis, convallis felis. Praesent sodales neque eget erat tincidunt, id iaculis est accumsan. Duis non erat congue, fermentum magna vel, interdum mauris. Phasellus at augue magna. Curabitur tempus, ligula maximus fermentum dapibus, velit mi dignissim elit, sit amet cursus nulla nisl vel magna. Sed ut nisi sit amet urna maximus laoreet vitae vitae quam. Ut malesuada est eu dapibus venenatis. Nulla lorem enim, viverra condimentum erat suscipit, blandit mollis eros. Fusce quis vulputate ligula. Praesent neque urna, vestibulum sed libero at, varius tincidunt turpis.",
            created_by_username: "user1",
            created_date_time: "9 Jun 2023 3:20:33 pm",
            last_edit_date_time: "9 Jun 2023 3:20:33 pm",
            image_url: "https://picsum.photos/200/300",
            image_caption: "This is a sample caption for image",
        },
    ];

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
    }

    let to_delete_moment;
    function confirmAndDeleteMoment(moment) {
        to_delete_moment = moment;
    }
</script>

<svelte:head>
    <title>Dashboard</title>
</svelte:head>

{#if to_delete_moment}
<section id="delete-dialog">
    <form>
        <p><b>You are about to delete the moment titled:</b></p>
        <h3>{to_delete_moment.title}</h3>
        <p><b>Deleting a moment will erase all data associated with this moment and cannot be recovered.</b></p>
        <p><b>Are you sure you want to delete this moment?</b></p>
        <p>Type <q>I want to delete this moment</q> in the below text box to confirm deletion and then click on Delete button</p>

        <input type="text" on:paste={() => false}>
        <button class="delete-dialog-button" on:click={() => { to_delete_moment = undefined; }}>Cancel</button>
        <button class="delete-dialog-button">Delete</button>
    </form>
</section>
{/if}
<section id="moments-header">
    <section id="moments-summary">
        <h3>Total moments: {moment_count}</h3>
        <button id="add-new-moment" on:click={() => goto("/addmoment")}>Add New Moment</button>
    </section>
    <section id="moments-search-bar-and-sort">
        <input type="search" id="searchbar" placeholder="Search title" />
        <button>Search</button>
        <section id="moments-sort-by">
            <label for="sort-by">Sort by</label>
            <select id="sort-by" name="sort-by">
                <option value="date-asc">Date posted (ascending)</option>
                <option value="date-desc">Date posted (descending)</option>
            </select>
        </section>
    </section>
</section>

<section id="moments-list">
    {#each moments_data as moment}
        <section class="moments-list-item">
            <section class="moments-list-item-image-section">
                <img
                    class="moments-list-item-image"
                    src={moment.image_url}
                    alt={moment.image_caption}
                />
            </section>
            <section class="moments-list-item-summary">
                <h3>{moment.title}</h3>
                <!-- This substring can be useful when description is too large
                    may improve memory consumption if a view of this string is used -->
                <span>{moment.description.substring(0, 200)}</span>
                <section class="moments-list-item-more-info">
                    <section class="moments-list-item-meta">
                        <span>Created {moment.created_date_time}</span> &#8226
                        <span>Last modified {moment.last_edit_date_time}</span>
                    </section>
                    <section class="moments-options">
                        <button class="moments-option-item" on:click={() => goto("/show/" + moment.id)}>&#128065</button>
                        <button class="moments-option-item" on:click={() => goto("/update/" + moment.id)}>&#128393</button>
                        <button class="moments-option-item delete-moment-option" on:click={() => confirmAndDeleteMoment(moment)}>&#128465</button>
                    </section>
                </section>
            </section>
        </section>
    {/each}
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
                }}
                class:moments-per-page-active={index ===
                    selected_index_for_moments_per_page}
                >{moment_per_page}</button
            >
        {/each}
    </section>
</section>

<style>
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
</style>
