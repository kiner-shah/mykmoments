import { loggedInUser } from "$lib/stores.js";
import { redirect } from "@sveltejs/kit";

export const ssr = false;

export function load() {
    const user = localStorage.getItem("loggedInUser");
    if (user) {
        const userObj = JSON.parse(user);
        loggedInUser.set(userObj);
        throw redirect(303, "/dashboard");
    }
}