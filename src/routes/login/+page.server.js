export const actions = {
    validateAndTryLogin: async ({request}) => {
        const formData = await request.formData();
        const username = formData.get('username');
        const password = formData.get('password');
        
        let result = fetch("/api/login", {
            method: "POST",
            body: JSON.stringify({username, password})
        }).then(response => {
            if (response.ok) {
                return response.body();
            }
            return response.statusText();
        });
    }
}