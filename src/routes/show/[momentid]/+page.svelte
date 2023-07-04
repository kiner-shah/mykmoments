<script>
    import { PUBLIC_API_URL } from "$env/static/public";
    import { loggedInUser } from "$lib/stores.js";
    import FixedStatusMessage from "$lib/FixedStatusMessage.svelte";

    export let data;

    async function getMoment(momentid) {
        var requestOptions = {
            method: 'GET',
            redirect: 'follow',
            headers: {
                "Authorization": "Bearer " + $loggedInUser.access_token
            }
        };

        const url = new URL("/getmoment", PUBLIC_API_URL);
        const url_search_params = new URLSearchParams({
            'id': momentid
        });
        url.search = url_search_params.toString();

        const response = await fetch(url.toString(), requestOptions);
        if (!response.ok) {
            throw new Error(response.statusText, {cause: response.status});
        }
        const json = await response.json();
        return json;
    }

    function getImageData(image_data, image_filename) {
        if (image_data === undefined || image_data.length == 0) {
            return "";
        }
        const file_extension = image_filename.split('.').pop();

        // Thanks to mobz (https://stackoverflow.com/a/9458996)
        // let binary = '';
        // let bytes = new Uint8Array( image_data );
        // var len = bytes.byteLength;
        // for (var i = 0; i < len; i++) {
        //     binary += String.fromCharCode( bytes[ i ] );
        // }

        let binary = "";
        for(let i = 0; i < image_data.length; i++) {
            binary += !(i - 1 & 1) ? String.fromCharCode(parseInt(image_data.substring(i - 1, i + 1), 16)) : ""
        }
        const base64_encoded = btoa( binary );
        return "data:image/" + file_extension + ";base64," + base64_encoded;
    }

    function convertToHumanReadableDateTimeString(input_date_time) {
        const event = new Date(input_date_time);
        return event.toLocaleString('en-GB', { dateStyle: 'long', timeStyle: 'medium', timeZone: 'IST', hourCycle: 'h12' });
    }
    
</script>

{#await getMoment(data.momentid)}
    <p>Loading...</p>
{:then moment} 
    <h1>{moment.title}</h1>
    <h3>{new Date(moment.date).toLocaleDateString('en-GB', { year: 'numeric', month: 'short', day: 'numeric' })}</h3>
    <figure>
        <img src={getImageData(moment.image_data, moment.image_filename)} alt={moment.image_caption} />
        <figurecaption>{moment.image_caption}</figurecaption>
    </figure>

    <p class="cursive-font">{@html moment.description}</p>
{:catch error}
    <p>{"cause" in error ? error.message : "Failed to retrieve info"}</p>
    <FixedStatusMessage is_error={true} message={"cause" in error ? error.message : "Failed to retrieve info"} />
{/await}


<style>
    h1 {
        text-align: center;
    }
    h3 {
        text-align: center;
        color: gray;
        font-style: italic;
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
        text-align: center;
    }
    .cursive-font {
        font-family: "LobsterTwo-Regular";
        text-align: justify;
        font-size: 20px;
    }
</style>