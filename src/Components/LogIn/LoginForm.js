
import React, {useState} from 'react';
import "../../Styles/LoginPage.css";

function LoginForm({Login, error}) {

    const [details, setDetails]=useState({email:"", password:""});

    const handler = event=>{
        event.preventDefault();
        Login(details)
    }
    return (
        <form 
        className = "LoginForm"
        onSubmit ={handler} 
        >
            <div className ="form-inner">
                <h2>Login to your Account</h2>
                {(error!="") ? (<div className= "error">{error} </div>):""}
             
                <div className = "form-group">
                    <lable htmlFor = "email">Email:</lable>
                    <input type = "email" name = "email" id = "email" onChange={event => setDetails({...details, email: event.target.value})} value ={details.email}/>
                </div>
                <div className = "form-group">
                    <lable htmlFor = "password">Password:</lable>
                    <input type = "password" name = "password" id = "password" onChange={event => setDetails({...details, password: event.target.value})} value ={details.password}/>
                </div>

                <input type = "submit" value ="Login"></input>
            
            </div>
        </form>
    )
}

export default LoginForm
