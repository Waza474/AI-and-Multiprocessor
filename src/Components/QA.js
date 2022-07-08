import { useState } from 'react';
import '../Styles/QA.css';

const QA = () => {
  const [messages, setMessages] = useState([]);
  let auth = true;
  
  const handleKeyDown = (e) => {
    if(e.key === "Enter"){
      let inputBox = document.getElementById("qaInput"); // Get input box in the page

      if(inputBox.value !== ""){ // If not empty
        setMessages(messages.concat([['q', inputBox.value]])); // Add the qa to the messages array
        inputBox.value = ""; // Reset input box to be clear

        document.getElementById("messageEnd").scrollIntoView({ behavior: "smooth" });
      }
    }
  }

  const respond = (e) => {
    let q = e.target.parentElement; // Get the question element
    let answer = prompt("Answer: ", "answer"); // Get answer

    // Create the answer element
    let answerNode = document.createElement("p");
    answerNode.className = "message a";
    let textNode = document.createTextNode(answer);
    answerNode.appendChild(textNode);

    q.appendChild(answerNode);
    document.getElementById("messageEnd").scrollIntoView({ behavior: "smooth" });
  }

  return (
    <div className="qa">
      <p className="qaHeader">Q&A</p>
      <div className="chatDisplay">
        {messages.map((m, mId) => {
          return (
            <p className={m[0] === 'q' ? 'message q' : 'message a'}>{m[0] === 'q' ? 'Q ' : 'A '}: {m[1]} {auth ? <a className="respond" onClick={respond}>Respond</a> : ''}</p>
          )
        })}
        <div style={{height: "40px", float: "left", clear: "both"}} id="messageEnd" />
      </div>
      <input type="text" placeholder="......" id="qaInput" onKeyDown={handleKeyDown} />
    </div>
  );
}

export default QA;
