#include <string>

const std::string html = R"html(
<html>
  <head>
    <style>
      html,
      body {
        margin: 0;
        padding: 0;
        height: 100%;
        width: 100%;
        font-family: sans-serif;
      }

      progress {
        margin: 0px;
        display: none;
      }

      progress:indeterminate {
        width: 100%;
      }

      .container {
        height: 100%;
        width: 100%;
        display: flex;
        flex-direction: column;
      }

      .content {
        display: flex;
        flex-direction: column;
        justify-content: center;
        flex-grow: 1;
      }

      .content-main {
        display: flex;
      }

      .content-origin {
        display: flex;
        flex-basis: 100%;
        flex-grow: 1;
        justify-content: center;
        align-items: center;
      }

      .content-draggable {
        height: 100px;
        width: 100px;
      }

      .content-dropzone {
        display: flex;
        flex-direction: column;
        flex-basis: 100%;
        flex-grow: 1;
        justify-content: center;
        align-items: center;
      }

      .dropzone-image {
        height: 100px;
        width: 100px;
      }

      .arrow {
        display: flex;
        align-self: center;
      }
    </style>

    <script>
      function onDragStart(event) {
        event.dataTransfer.setData('text/plain', event.target.id);
      }

      function onDragOver(event) {
        event.preventDefault();
      }

      function onDrop(event) {
        const dropzone = event.target;

        const draggableElement = document.getElementById(event.dataTransfer.getData('text'));
        const progressElement = document.getElementById('progress');

        // dropzone.appendChild(draggableElement);
        
        draggableElement.style.display = 'none';
        progressElement.style.display = 'block';

        event.dataTransfer.clearData();

        window.install()
          .then(result => {
            // TODO: handle result
          })
          .catch(err => {
            draggableElement.style.display = 'block';
          })
          .finally(() => (progressElement.style.display = 'none'));
      }
    </script>
  </head>
  <body>
    <div class="container">
      <div class="content">
        <div class="arrow">
          <svg xmlns="http://www.w3.org/2000/svg" width="65.76062" height="20.94" viewBox="0 0 65.76062 20.94" creator="Katerina Limpitsouni"><path d="M3.0696,14.35003c14.77852-4.66461,30.54831-5.6899,45.76236-2.61603,4.44585,.89825,8.8161,2.1243,13.095,3.6261,1.274,.44715,2.73515-.50817,3.0753-1.7461,.37901-1.37937-.46745-2.62652-1.7461-3.0753C47.82195,5.12165,31.2588,3.6356,15.11839,6.30519c-4.53218,.74961-8.99807,1.84099-13.37799,3.22345C-1.31727,10.49374-.01125,15.32246,3.0696,14.35003h0Z" fill="#01010C" origin="undraw"/><path d="M55.97417,3.76117l2.58581,4.86421,1.32984,2.5016,.66492,1.2508c.08649,.16269,.16913,.42327,.29552,.55591-.51452-.53997,1.42541-2.15361,.51312-1.82543-.8054,.28974-1.58804,.8423-2.34456,1.24356-.82749,.4389-1.65498,.87781-2.48247,1.31671-1.65498,.87781-3.30996,1.75562-4.96494,2.63342-1.15699,.61367-1.60767,2.31798-.89688,3.42047,.75851,1.17651,2.18291,1.55329,3.42047,.89688,1.93081-1.02411,3.86162-2.04822,5.79243-3.07233,.91943-.48767,1.83887-.97534,2.7583-1.46301,1.14784-.60882,2.34415-1.1119,2.8815-2.38522,.54904-1.30103,.0367-2.54313-.58127-3.70559l-1.55149-2.91853c-1.03432-1.94569-2.06865-3.89137-3.10297-5.83706-.61473-1.15638-2.31725-1.60814-3.42047-.89688-1.17565,.75796-1.55443,2.18355-.89688,3.42047h0Z" fill="#01010C"/></svg>
        </div>
      
        <div class="content-main">
          <div class="content-origin">
            <img
              id="draggable-1"
              class="content-draggable"
              draggable="true"
              ondragstart="onDragStart(event);"
              src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAGAAAABgCAYAAADimHc4AAAACXBIWXMAAAsTAAALEwEAmpwYAAADIUlEQVR4nO2aPWsUURiFDyg22gv+Af0NWvkDrCRpTFprIUZJZsKMiiKioIU6fjQKiriK+RJLOwt3NmwSjWjixspKC90ZNWByJARBFHVmd+7cm53zwNvu7pwz97zvvXcBIYQQQgghhBBCCCEMwxfBNtbDa4xDFlYLN8h3kxlrgly4Wtx3xyG/T/Q93RQvDhvBLtaDZ8U9/Any9a184s9fKlz81AfhOnwe7GUcvC/s4RunyMW72cVffkTOnjcivvMGsBEcZj1cKezhZ06TS/eyi996SDbPGRPfWQOM5H3zLNl6kF38pfvkzBmj4jtpQPF5H5KzFzaiJKv4i3c2osqw+M4ZUHjexyE5d5FcHs8u/pvbZONkKeI7ZUDheR+H5MvLOSadSfLVzUKF/5/4ThhgJO9j+zN+FvGtG2Ak72P7M35W8a0aYCTvG/Zn/DziWzPASN7P2J/x84pfugHG8r5pf8bvRPxSDTCT96ETM36n4pdmgJG8j92Y8bsRvxQDjOR97MaM36345RgQgUbqyYF8Zeh3dCO+DIhkgFaAaRRBUARRPUBNOFUThqYgjaHQGErtA7QRSzfjRmz1Cjh3FJwaBGv9ndX0ADg3vPFZ2gnnNGBduFp/MbX+WTIgpwHdvPm132pqQAbkfgPX46NWkAHTgzLAagTNH5MBnTXh4e5WgppwD1Za2fuAyI2SAZEM0ArwFUFUBKkHUE04UhPWFORrDKXGUO0DqI1YpI2YdsK+jiKoowgHzmuosyD7QlGHcfbFok5De69S3QdABuhCBroRoyJIV5Kp7oSxqZpw4uObLuUjOwYkPtYSD4MyILJmwHHj4utvKfibAddLEV8G4E/xPTxmgK0yILLQhD3EDLCjNPG1AvBr5rfaI9hZqvgyAD/f/I+fR7CndPFlAJh4WGmPYb8V8atuQOJjre3hkDXxq25A28eQVfGrbEDiI4ILVNIAD1Pswxa4QNUMSHzUOYTtcIUqGZB4eGtl1v8XFTLgwycfu+EaVTAg8fD1yxj2wUVsC0TDlXhYTUdxEK7S8ytgFEdsayyEEEIIIYQQQggh0JP8AFnphK40ywd0AAAAAElFTkSuQmCC"
              alt="file"
            />
          </div>

          <div class="content-dropzone" ondragover="onDragOver(event);" ondrop="onDrop(event);">
            <img
              class="dropzone-image"
              src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAGAAAABgCAYAAADimHc4AAAACXBIWXMAAAsTAAALEwEAmpwYAAADcklEQVR4nO2cvWsUQRjGJ8ZEQU1A0cIg+HGX2ZyCFmJtJVhqYWNra2trSkGiCVYRwk1IwBDRaySCQQORxN1JLMTYWIgo2PiBaCq/HtkIIZEdk7u9m3fMPT94/4B5fzM7s8/ejVKEEEIIIYQQQgghhBBCCCF/AaMOw6jrKKsFlNUijEJdq6x62fQMMK7aUVY3UFY/6t50Qwnraf5kwxtvuBKyBaQz32fzDR9Hq5/5Ph47hhJcs79frPlmeSVcadqNGUa9EBdgmvh0hLL6Kt5808QrQbzpJrBK333+vANdw5AqUYARlZEeTgYwqNq4AozoynjQMAniS978J1VW/RRgRAV8x7Dq4QowohL6KMCICnhOAUZ0L/hCAUZUACjAUEBTl+J7gKIA5wy5uwN42AXMFACrARttvEr0NyT6NayuIOk+h3HVKv8mfGsLML1fvjlWpOZhoyNyAsa2AnFRugmQLf0JiT7uX8BIKzBzKIAGRPKV6DeYP9jpV8DEbvmB24Aqia76FTBbCGPgNpBKovdYKLX7EZBuvNIDtgFWUjrmR0ClQ36wNsBKojN+BNzfIz9YG2DF0Sk/Aqb2yQ/WBlnVvRNwA47q+5bsZRMe3Sw9yxBoPauq+TULuL1NeqAIspJoxI+AezvlB2sDrLjnkh8Bk3vlB2tDrO7TfgRMHwhgsFF4NVvsaryA4ZZ0t5cfrA2t9Meqm1+TgDFGEMgSkERTfgRUOgOYbVGINeBHACMIOFbABT8CHjGCQJaAueIJPwL4DQAZs/8nFkrbGy+AEQQcJ6CXNTW/agGMIJD9/Nd3/AhgBAGHgF4/AhhBwHEEPetHACMIZAp4Wio0XgAjCDgeP4uA2tR4AYwg4BAQ19z8qgQwgoDjCHrTjwBGEHCsgIt+BDCCQHYEoU/6EcAIAo4QblfjBTCCgOP8/zZX89ctgBEEHLN/wo8ARhBwnIDy32+U/vmYEURU21ewRJ/PLyC9nGitFfCYv4JAloC4lP9Cp6WboRhBoIbZ/w5QLfkFDKnSP6+tZAQBh4TLuZu/LCG9losRBKpYAa8QFzrqJ2BQtS1dy8UIAms3X39AXDxat+avkpBe4preDLVSBP+IgRUC5mC1rnvzM/aEvvRyoqU7RZs5gkj0Lzzp/oxEV9KvXrlyf0IIIYQQQgghhBBCCCGEqI3Kb31lQ9ZXmS7xAAAAAElFTkSuQmCC"
              alt="folder"
            />
          </div>
        </div>
      </div>

      <progress id="progress"></progress>
    </div>
  </body>
</html>
)html";

std::string command_as_root(std::string command)
{
  return R"(
install_command=')" +
         command + R"('
if [ "$XDG_SESSION_TYPE" = "wayland" ] ; then
    xhost +SI:localuser:root
    pkexec ${install_command}
    xhost -SI:localuser:root
    xhost
elif command -v pkexec >/dev/null 2>&1; then
    pkexec ${install_command}
elif command -v sudo >/dev/null 2>&1; then
    x-terminal-emulator -e "sudo ${install_command}"
elif command -v su >/dev/null 2>&1; then
    x-terminal-emulator -e "su - -c '${install_command}'"
else
    x-terminal-emulator -e "echo 'Command must be run as root user: ${install_command}'"
fi
)";
}