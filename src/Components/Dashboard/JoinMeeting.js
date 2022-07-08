import { Component } from "react";


class JoinMeeting extends Component{
    render(){
        return(
        <div className="join_meeting">
            <form>
            <div className = "join_meeting_form">
                <h2>Join meeting</h2>
                <div className = "form1-group">
                    <lable htmlFor = "Metting ID">ID:</lable>
                    <input type = "text" name = "meeting_id" id = "id"/>
                </div>

                <div className = "form1-group">
                    <lable htmlFor = "Metting Password">Password:</lable>
                    <input type = "text" name = "meeting_password" id = "metting_pwd"/>
                </div>

                <button id ="joinmeeting">Join Meeting </button>
                
                </div>
            </form>
            </div>
        )
    }
    


}

export default JoinMeeting;