<script>
    /*
    Quotes 
    1. Life is short and every moment is precious - Gad Saad
    2. We do not remember days, we remember moments - Cesar Pavese
    3. The little things? The little moments? They aren't little. - Jon Kabat-Zinn
    */
    const quotes = [
        {
            quote_img: "quote_img/quote1.jpg",
            quote_img_credits:
                "Photo by Simon Berger: https://www.pexels.com/photo/landscape-photography-of-mountain-722939/",
            quote_msg: "Life is short and every moment is precious",
            quote_by: "Gad Saad",
        },
        {
            quote_img: "quote_img/quote2.jpg",
            quote_img_credits:
                "Photo by David Bartus: https://www.pexels.com/photo/photo-lavender-flower-field-under-pink-sky-1166209/",
            quote_msg: "We do not remember days, we remember moments",
            quote_by: "Cesar Pavese",
        },
        {
            quote_img: "quote_img/quote3.jpg",
            quote_img_credits:
                "Photo by Matej: https://www.pexels.com/photo/close-up-photography-of-vintage-watch-1034425/",
            quote_msg:
                "The little things? The little moments? They aren't little",
            quote_by: "Jon Kabat-Zinn",
        },
    ];

    let slideIndex = 0;
    let intervalId;
    const timeout = 10000; // show each slide for 10 seconds

    function showNextSlide() {
        slideIndex = (slideIndex + 1) % quotes.length;
    }

    intervalId = setInterval(showNextSlide, timeout);
</script>

<section class="slideshow">
    {#each quotes as quote, index}
        <section
            class="slide fade"
            style="display: {index === slideIndex ? 'block' : 'none'}"
        >
            <img class="slide-image" src={quote.quote_img} alt="quote{index}" />
            <p class="slide-quote">{quote.quote_msg}</p>
            <p class="slide-quote-by">by {quote.quote_by}</p>
        </section>
    {/each}

    <section class="slide-dots">
        {#each quotes as { }, index}
            <span class="dot" class:active={index === slideIndex} />
        {/each}
    </section>
</section>

<style>
    .slideshow {
        width: 100%;
        height: 100%;
    }
    .slide {
        position: relative;
        color: white;
    }
    .slide-image {
        width: 100%;
        height: 80%;
        object-fit: cover;
    }
    .slide-quote {
        font-size: 4em;
        text-align: center;
        position: absolute;
        top: 15px;
        padding: 2%;
    }
    .slide-quote-by {
        font-size: 2em;
        font-style: italic;
        position: absolute;
        text-align: center;
        bottom: 10%;
        width: 100%;
    }
    .slide-dots {
        text-align: center;
    }
    .dot {
        width: 15px;
        height: 15px;
        background-color: #bbbbbb;
        border-radius: 50%;
        display: inline-block;
        transition: background-color 0.6s ease;
        margin: 0px 2px;
    }
    .active {
        background-color: #717171;
    }
    .fade {
        animation-name: fade;
        animation-duration: 1.5s;
    }

    @keyframes fade {
        from {
            opacity: 0.4;
        }
        to {
            opacity: 1;
        }
    }
</style>
