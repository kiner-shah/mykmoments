<script>
    import SlideShow from '$lib/SlideShow.svelte';

    const homePageImages = [
        {
            img_path: "home_page/image0.jpg",
            img_credits: "Photo by Andre Furtado: https://www.pexels.com/photo/a-woman-sits-on-a-rock-beside-the-lake-2916820/"
        },
        {
            img_path: "home_page/image1.jpg",
            img_credits: "Photo by Stan Swinnen: https://www.pexels.com/photo/unrecognizable-traveler-standing-on-mountain-top-and-admiring-landscape-6465964/"
        },
        {
            img_path: "home_page/image2.jpg",
            img_credits: "Photo by Lisa Fotios: https://www.pexels.com/photo/person-holding-photo-of-plant-2769188/"
        }
    ];
</script>

<svelte:head>
    <title>Home</title>
</svelte:head>

<!--<main class:red="{data.status !== 200}">
    <h2>{data.status}</h2>
    <h3>{data.body}</h3>
</main>-->

<SlideShow />

<section>
    <h2>Store your special moments at one place</h2>
    
    <section class="home-page-section">
        <img class="home-page-image" src={homePageImages[0].img_path} alt="image0" />
        <h3>Describe your special moment</h3>
    </section>
    <section class="home-page-section">
        <img class="home-page-image" src={homePageImages[1].img_path} alt="image1" />
        <h3>Describe your feelings for this moment</h3>
    </section>
    <section class="home-page-section">
        <img class="home-page-image" src={homePageImages[2].img_path} alt="image2" />
        <h3>Add a photo to remember this moment</h3>
    </section>

    <button id="sign-up-button" on:click={() => {
        window.location.href = "/createaccount";
    }}>Sign Up</button>
</section>

<style>
    h2 {
        font-size: 4em;
        text-align: center;
        margin-left: 1%;
        margin-right: 1%;
    }
    .home-page-section {
        display: flex;
        flex-direction: column;
        column-gap: 30px;
        align-items: center;
        margin-bottom: 3%;
    }
    .home-page-image {
        width: 50%;
        border-radius: 50%;
    }
    #sign-up-button {
        padding: 1% 2% 1% 2%;
        border-style: none;
        display: block;
        background-color: rgb(250, 229, 107);
        color: rgba(144,128,39,1);
        margin: 0% auto 2% auto;
    }
    #sign-up-button:hover {
        background-color: rgb(238, 218, 103);
    }
</style>