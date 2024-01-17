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

      * {
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
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
            const status = result?.status === 0;
            if (!status) {
              draggableElement.style.display = 'block';
            }
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
              src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAGQAAABkCAYAAABw4pVUAAAACXBIWXMAAAsTAAALEwEAmpwYAAAFX0lEQVR4nO2dbUxbVRjHS3yL30zUSAkvXwyf/GI0UZibwAQmkTiHGy9BEFFkU96Zm4wUsmWZmCiZjDmIxKgxmum2DxJP1PgSN8myTOikZiabLhkOcQba51w/GJn+zelGUtpKC7Q8t+z5J/+v957z+91zz22b9DocEolEIpFIJBKJRCKRxDNer/c2ImrVWp/SWnu11kiwes3YiajFzCVhrxYAN2mt64jodxtARYw6rbXeAeAWRyKFiIq11udsABDxKBFdJKIqAEkOO4eIHiCi49zA9MrV3IYfdtgtRJRJRIeJ6F8bQALDivnCsqx77CDidiJ6hYj+WmjAPp8PRw5/jfKSbuRkNSZUy0u6/WM3c4gg5W8iGrAs664VFwHgZiJq0lr7Il09334zimcq97GDzVlmnyrdg0+HR6JZLX8SUTeAW1dCRJJlWZu11hciDezH8fPY3tzPDjInxm3ath9joz9FFOPz+S6Zp0wAN8RFhmVZeUQ0FmkgF375Ffv2vIe8Nc3zJlLwUAP2ltfidGcZPF1bEqJjnaXora7BhrUN8+aSm92Ezh2DOH/uYjRi3JZlrY+ZCAA3aq1fj7Rh/3F5Gm+/NYwNue1Bg2/ES5uew8jOxBHhCerJl8vQVfos8rLniynMaUNf78eY+u1ypNuYYfeaYblsIVrr3oVO5vX68NGHX2JjUUfI8q5/rB6ft1WwA/XEqF9tr0DrxrqQeRYX7vRfjNPTMwuKmZiYOLgsGUSUFWnDri7fG7oBFm7F0cZKdoCeOPWT5krUPLo1ZN4Vm3f7N34i+t+VMjQ0WOVwOJb2gVJr/X64A39/+ixefL43ZEBP5L6Agdqn8YOLH5onzh13bcG79VUoydsWwqGu5lWcHBkPK2VsbPR4RkbKvUsV8nPwAd/sO+bf1AIHULS2Afura+B2lbKD8qxwzZzN3IvCbPyHDhwLETI5OTmVmuosdzqddyxaCBHNBh/QbGRzJ12/pgG7y2pxqiNxN2xPjGoYGBaGSeCmH+ap64oRkpGRkrWUFRJywMCr4OyBdsy8I50JYGCYBDIKx9AISUtLKY65EKgeqQplEI2Q1NSUMhGi7CTEWS5ClAi5Lm9pObJCetgliBDFD16EKH7YIkTxAxYhih+qCLEBSIiQ8BBmh3twqL0DJfnzf3WMRUsKWjDQtst/DhES5RU40LYr7r+Nm3OIkCiFlMRhZQR3U36LCIlaSEFL3IU8WSBCbHXLGmzvkBUSrZDZ4atS4rFSZFO3wWMo5LGXHwpECD8I2KSBt0j5gUqJEPYrEjarrBDFL0GEKH7wIkTxwxYhih+wCFH8UEWIDUBChPDDgwjhBwYRwg8JIoQfDEQIPwzYoPLVieKXIEIUP3gRovhhixDFD1iEKH6oIsQGICFC+OFBhPADgwjhhwQRwg8GIoQfBmxQ+epE8UsQIYofvAhR/LBFiOIHLEIUP1QRYgOQECH88CBC+IFBhPBDggjhBwMRwg8DNqh8daL4JYgQxQ9ehCh+2CJE8QNOGCHSxogMYiqEiK4EH+yRdfH/p56cVdL8da3h/oz/n+UIuRR8wP43joiUrMgyzIV7sO9oiJCpqanp5Qj5YKE37Ej1ohm43aMnlvxn/ET04PX69k4dp3Z3u7quvq7C+fiihVyT0s89Cb1KOjIy8tnc7WpJL3QxMa96m5ycHOKejF4FMjIz766cE5KcnHynYzkZGhqqdbvd35lNyTwpcE9Q27yGkWF15oz7hMvV6b9NzTU9Pf0+RwySlJaWdn/ggaXORTO4JiN272E3Sy09PSXbbErmSUGkOCNISCkzrAyzJb2ZTSKRSCQSiUQicay6/AfiYwOyRxZpIwAAAABJRU5ErkJggg=="
              alt="file"
            />
          </div>

          <div class="content-dropzone" ondragover="onDragOver(event);" ondrop="onDrop(event);">
            <img
              class="dropzone-image"
              src="data:image/png;base64,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"
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
