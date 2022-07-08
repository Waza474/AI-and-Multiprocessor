import { useState } from 'react';
import '../Styles/Chat.css';

function Chat() {
  const [messages, setMessages] = useState([]);

  const handleKeyDown = (e) => {
    if(e.key === "Enter"){
      let inputBox = document.getElementById("chatInput"); // Get input box in the page

      if(inputBox.value !== ""){ // If not empty
        const randNum = Math.floor(Math.random() * 2);
        setMessages(messages.concat([[randNum, inputBox.value]])); // Add the message to the messages array
        inputBox.value = ""; // Reset input box to be clear

        document.getElementById("messageEnd").scrollIntoView({ behavior: "smooth" });
      }
    }
  }

  return (
    <div className="chat">
      <p className="chatHeader">Chat Box</p>
      <div className="chatDisplay">
        {messages.map((m, mId) => {
          return (
            <p className={m[0] === 0 ? 'message sent' : 'message received'}>{m[0]}: {m[1]}</p>
          )
        })}
        <div style={{height: "40px", float: "left", clear: "both"}} id="messageEnd" />
      </div>
      <input type="text" placeholder="......" id="chatInput" onKeyDown={handleKeyDown} />
    </div>
  );
}

export default Chat;