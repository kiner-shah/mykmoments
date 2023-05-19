import { loggedInUser } from "$lib/stores.js";
import { redirect } from "@sveltejs/kit";

export const ssr = false;

export function load() {
    const user = localStorage.getItem("loggedInUser");
    if (user) {
        loggedInUser.set(user);
        throw redirect(303, "/");
    }
}