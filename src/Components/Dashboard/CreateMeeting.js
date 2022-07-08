import { Component } from "react";


class CreateMeeting extends Component{
    render(){
        return(
        <div className="create_meeting">
            <form>
            <div className = "create_meeting_form">
                <h2>Create your meeting</h2>
                <div className = "form1-group">
                    <lable htmlFor = "Metting ID">ID:</lable>
                    <input type = "text" name = "meeting_id" id = "id"/>
                </div>

                <div className = "form1-group">
                    <lable htmlFor = "Metting Password">Password:</lable>
                    <input type = "text" name = "meeting_password" id = "metting_pwd"/>
                </div>

                <button id ="createroom">Create Room </button>
                
                </div>
            </form>
            </div>
        )
    }
    


}

export default CreateMeeting;