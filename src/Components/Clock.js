import { useState, useEffect } from "react";

const Clock = () => {
  const [time, setTime] = useState("00:00:00");

  useEffect(() => {
    setInterval(() => {
      const date = new Date();

      // Set hours (add leading 0 if needed)
      let currTime = String(date.getHours()).length === 1 ? "0" + date.getHours() : date.getHours();
      // Set minutes (add leading 0 if needed)
      currTime += String(date.getMinutes()).length === 1 ? ":0" + date.getMinutes() + ":" : ":" + date.getMinutes() + ":";
      // Set seconds (add leading 0 if needed)
      currTime += String(date.getSeconds()).length === 1 ? "0" + date.getSeconds() : date.getSeconds();

      setTime(currTime);
    }, 1000);
  }, []);

  return (
    <div>
      {time}
    </div>
  );
}

export default Clock;
