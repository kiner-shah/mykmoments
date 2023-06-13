<script>
    export let data;
</script>

<h1>{data.moment.title}</h1>
<figure>
    <img src={data.moment.image_url} alt={data.moment.image_caption} />
    <figurecaption>{data.moment.image_caption}</figurecaption>
</figure>

<p>{@html data.moment.description}</p>

<style>
    h1 {
        text-align: center;
    }
    figure {
        display: flex;
        flex-direction: column;
        text-align: center;
        justify-content: center;
        margin-left: auto;
        margin-right: auto;
    }
    figure img {
        object-fit: contain;
    }
    figurecaption {
        color: gray;
    }
    p {
        font-family: "LobsterTwo-Regular";
        text-align: justify;
        font-size: 20px;
    }
</style>