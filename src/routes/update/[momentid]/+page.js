import { loggedInUser } from "$lib/stores.js";
import { error, redirect } from "@sveltejs/kit";

export const ssr = false;

// TODO: replace with data fetched from backend
let moments_data = [
    {
        id: "moment0",
        title: "Title 0",
        description:
            "This is a very long summary for this moment.<br> I have no idea what to do to get this summary, so I had no choice but to use this dummy",
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

export function load({params}) {
    const user = sessionStorage.getItem("loggedInUser");
    if (!user) {
        throw redirect(303, "/login");
    }
    loggedInUser.set(JSON.parse(user));

    const moment = moments_data.find(m => m.id === params.momentid);
    if (!moment) {
        throw error(404, "Not Found");
    }
    return {moment};
}