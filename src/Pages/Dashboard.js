import { Component } from "react"
import {useHistory} from "react-router-dom";
import "../Styles/Dashboard.css";


function Dashboard(){
    let his = useHistory();
    
    const create_meeting =()=> {
        console.log ("create meeting");
        his.push("./createmeeting")
    }

    const join_meeting =()=> {
        console.log ("join meeting");
        his.push("./joinmeeting")
    }
    
    const setting =()=> {
        console.log ("setting");
        his.push("./createmeeting")
    }
    
    
            return(
                <div className ="DashBoard">
                    <div className ="create-meeting">
                        <button id= "create_meeting_btn" onClick={create_meeting}> Create Meeting </button>
                    </div>

                    <div className ="join-meeting">
                        <button id = "join_meeting_btn" onClick={join_meeting}> Join Meeting </button>
                    </div>

                    <div className ="setting">
                        <button id = "setting_btn" onClick={setting}> Setting</button>
                    </div>
                </div>

              
        
            )
        }
export default Dashboard;