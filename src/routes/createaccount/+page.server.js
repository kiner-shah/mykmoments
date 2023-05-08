export const actions = {
    validateAndCreateAccount: async ({request}) => {
        const formData = await request.formData();
        const emailid = formData.get('emailid');
        const username = formData.get('username');
        const password = formData.get('password');
        const confirmpassword = formData.get('confirmpassword');
        
    }
}