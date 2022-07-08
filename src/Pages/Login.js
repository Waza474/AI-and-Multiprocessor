import React, {useState} from 'react';
import LoginForm from '../Components/LogIn/LoginForm';
import Dashboard from './Dashboard'


function Login(){
  const adminUser = {
    email: "admin@gmail.com",
    password: "admin"
  }

  const [user, setUser] = useState({email:""});
  const [error, setError] = useState("");

  const LoginDetails = details => {
    console.log(details);
    if(details.email == adminUser.email && details.password == adminUser.password){
      console.log("logged in");
      setUser({
        email:details.email
      });
    }
    else{
      console.log("unmatched");
      setError("Incorrect Password or Email")
    }
  }

  const Logout =() =>{
    console.log("logout");
    setUser({email:""})
  }
    return (
      <div className ="login">
        {(user.email!="")? (
          <div className="welcome">
          <h2> Welcome To New-Gen Virtual Classroom</h2>
          <Dashboard/>
          <button id= "logout" onClick={Logout}> LogOut</button>
          </div>
      ):(
      <LoginForm Login={LoginDetails} error ={error}/>

      )}
    </div>
  )
}
export default Login;
