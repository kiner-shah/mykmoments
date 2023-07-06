import { loggedInUser } from "$lib/stores.js";
import { redirect } from "@sveltejs/kit";

export const ssr = false;

export function load({params}) {
    const user = localStorage.getItem("loggedInUser");
    if (!user) {
        throw redirect(303, "/login");
    }
    loggedInUser.set(JSON.parse(user));

    return {momentid: params.momentid};
}